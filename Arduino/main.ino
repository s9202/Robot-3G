//#include <WProgram.h>
//#include<Arduino.h>
#include <Servo.h>
#include "funkcje.h"



char pozycjaRobota = PRZODEM_GORA;

Wezel mapa[ROZMIAR_MAPY];
int miejsceRobota = 0;

Servo servo1;
Servo servo2;
Servo servo3;

//Zmienne globalne dla Arduino
char odebranyBajt;			//Zmienna przechowująca bajt pobrany z SerialPortu
bool jestTrybAuto = false;	//Tryb pracy robota (automatyczny = true)
//Zmienne dla trybu auto
//Wyznaczenie celu. Teraz stałe, później z algorytmu.
int cel;
bool osiagnietoCel = true;
bool wykrytoElement = false;
bool jestDroga = false;
int licznik = 0;




//--------------------------- wybranie portów które będziemy używać
void setup() {
	pinMode(13, OUTPUT);                //Port13 to akurat jest wbudowana dioda
	pinMode(11, OUTPUT);  //dla piezo
	digitalWrite(13, LOW);        //port13
	Serial.begin(9600);                //Uruchomienie USB serialport z prędkością 9600

	//Tworzenie mapy początkowej
	inicjujMape(ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, miejsceRobota, pozycjaRobota, mapa);
	wyslijStringJson(mapa, ROZMIAR_MAPY);
}

//------------------------------ Główna pętla z programem

void loop() {

	//Początek pętli zawsze badanie bajtu na serialport
	if (Serial.available() > 0) {
		odebranyBajt = Serial.read();

		switch (odebranyBajt) {
			case (TRYB_MANUAL):
				jestTrybAuto = false;
				break;
			case (TRYB_AUTO):
				jestTrybAuto = true;
				break;
			case (JAZDA_PRZOD):
				if (!jestTrybAuto) {
					jedzProsto(servo1, servo2);
				}
				break;
			case (JAZDA_LEWO):
				if (!jestTrybAuto) {
					skrecajWLewo(servo1, servo2);
				}
				break;
			case (JAZDA_PRAWO):
				if (!jestTrybAuto) {
					skrecajWPrawo(servo1, servo2);
				}
				break;
			case (JAZDA_TYL):
				if (!jestTrybAuto) {
					jedzDoTylu(servo1, servo2);
				}
				break;
			case (JAZDA_PRZOD_SKOK):
				if (!jestTrybAuto) {
					miejsceRobota = wykonajJedenRuchPrzod(mapa, miejsceRobota, pozycjaRobota, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY);
				}
				break;
			case (JAZDA_LEWO_SKOK):
				if (!jestTrybAuto) {
					pozycjaRobota = wykonajObrot90Lewo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY);
				}
				break;
			case (JAZDA_PRAWO_SKOK):
				if (!jestTrybAuto) {
					pozycjaRobota = wykonajObrot90Prawo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY);
				}
				break;
			case (JAZDA_TYL_SKOK):
				if (!jestTrybAuto) {
					miejsceRobota = wykonajJedenRuchTyl(mapa, miejsceRobota, pozycjaRobota, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY);
				}
				break;
			case (JAZDA_STOP):
				if (!jestTrybAuto) {
					zatrzymajRuch(servo1, servo2);
				}
				break;
			case (GLOWA_LEWO):
				glowaLewo(servo3);
				break;
			case (GLOWA_PRAWO):
				glowaPrawo(servo3);
				break;
			case (GLOWA_STOP):
				zatrzymajGlowe(servo3);
				break;
			case (SKAN_MAN):
				skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
				//wyslijTablice(mapa, ROZMIAR_MAPY);
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				break;
		}
		//Dla trybu auto rozpoczęcie samodzielnego badania terenu
		if (jestTrybAuto) {
			if (osiagnietoCel) { //przed ustawieniem celu skanowanie otoczenia
				wykrytoElement = skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				pozycjaRobota = wykonajObrot90Prawo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
				
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				wykrytoElement = skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
				
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				cel = wyznaczCel(mapa);
				
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				osiagnietoCel = false;
				wykrytoElement = true;
				jestDroga = false;
			}
			if (licznik == 4) { //po ilu ruchach bez przemieszczenia porzuca cel
				osiagnietoCel = true;
			}
			if (!osiagnietoCel && wykrytoElement && !jestDroga) {
				pozycjaRobota = wykonajObrot90Prawo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
				
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				ustalSasiadow(mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
				wyznaczTrase(miejsceRobota, cel, mapa);
				
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				wykrytoElement = false;
				jestDroga = true;
			}
			if (!osiagnietoCel && !wykrytoElement && jestDroga) {
				Robot robot;
				robot = wykonajRuchDoCelu(mapa, miejsceRobota, pozycjaRobota, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, servo1, servo2);
				if (robot.miejsceRobota == cel) {
					osiagnietoCel = true;
					licznik = 0;
				}
				if (miejsceRobota == robot.miejsceRobota) {
					jestDroga = false;
					pozycjaRobota = wykonajObrot90Prawo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
					
					wyslijStringJson(mapa, ROZMIAR_MAPY);
					licznik++;
				} else {
					licznik = 0;
				}
				miejsceRobota = robot.miejsceRobota;
				pozycjaRobota = robot.pozycjaRobota;
				
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				if (!osiagnietoCel) {
					wykrytoElement = skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
					if (wykrytoElement) {
						jestDroga = false;
					}
					
					wyslijStringJson(mapa, ROZMIAR_MAPY);
				}
			}
		}
	}
}

