//#include <WProgram.h>
//#include<Arduino.h>
#include <Servo.h>
#include "funkcje.h"

//Mapa obszaru dzialania robota
Wezel mapa[ROZMIAR_MAPY];

//Zmienna zawierajaca aktualna pozycje robota wzgledem mapy
char pozycjaRobota = PRZODEM_GORA;

//Zmienna zawierajaca aktualne miejsce robota na mapie
int miejsceRobota = 0;

//Zmienna zawierajaca aktualne miejsce celu na mapie
int cel;

//Tablica przechowujaca indeksy celow
int tablicaCelow[ROZMIAR_MAPY];

/*
*servo1 Kolo lewe?
*servo2 Kolo prawe?
*servo3 Glowa z kamera
*/
Servo servo1;
Servo servo2;
Servo servo3;

//Zmienna przechowująca bajt pobrany z SerialPortu
char odebranyBajt;

//Tryb pracy robota (automatyczny = true)
bool jestTrybAuto = false;	

//Warunki dla trybu automatycznego (stan na starcie lub po osiągnięciu celu)
//Oznaczenie czy wykonano wszystkie cele z aktualnej puli celow
bool zbadanoMape = false;
//Oznaczenie czy obecny cel zostal osiagniety lub gdy zostal porzucony
bool osiagnietoCel = true;
//Warunek wykonania ruchu (gdy jest droga i gdy nie wykryto elementu)
bool moznaWykonacRuch = false;
//Licznik określający liczbę nieudanych znalezien trasy do celu
int licznik = 0;




//--------------------------- wybranie portów które będziemy używać
void setup() {
	pinMode(13, OUTPUT);                //Port13 to akurat jest wbudowana dioda
	pinMode(11, OUTPUT);  //dla piezo
	digitalWrite(13, LOW);        //port13
	Serial.begin(9600);                //Uruchomienie USB serialport z prędkością 9600

	//Tworzenie mapy początkowej
	miejsceRobota = inicjujMape(ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, miejsceRobota, pozycjaRobota, mapa);
	wybierzCeleA(tablicaCelow, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
	wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
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
				wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
				break;
			case (JAZDA_PRZOD):
				if (!jestTrybAuto) {
					miejsceRobota = inicjujMape(ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, miejsceRobota, pozycjaRobota, mapa);
					jedzProsto(servo1, servo2);
				}
				break;
			case (JAZDA_LEWO):
				if (!jestTrybAuto) {
					miejsceRobota = inicjujMape(ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, miejsceRobota, pozycjaRobota, mapa);
					skrecajWLewo(servo1, servo2);
				}
				break;
			case (JAZDA_PRAWO):
				if (!jestTrybAuto) {
					miejsceRobota = inicjujMape(ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, miejsceRobota, pozycjaRobota, mapa);
					skrecajWPrawo(servo1, servo2);
				}
				break;
			case (JAZDA_TYL):
				if (!jestTrybAuto) {
					miejsceRobota = inicjujMape(ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, miejsceRobota, pozycjaRobota, mapa);
					jedzDoTylu(servo1, servo2);
				}
				break;
			case (JAZDA_PRZOD_SKOK):
				if (!jestTrybAuto) {
					miejsceRobota = wykonajJedenRuchPrzod(mapa, miejsceRobota, pozycjaRobota, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
				}
				break;
			case (JAZDA_LEWO_SKOK):
				if (!jestTrybAuto) {
					pozycjaRobota = wykonajObrot90Lewo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
				}
				break;
			case (JAZDA_PRAWO_SKOK):
				if (!jestTrybAuto) {
					pozycjaRobota = wykonajObrot90Prawo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
				}
				break;
			case (JAZDA_TYL_SKOK):
				if (!jestTrybAuto) {
					miejsceRobota = wykonajJedenRuchTyl(mapa, miejsceRobota, pozycjaRobota, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, servo1, servo2);
					wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
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
				if (!jestTrybAuto) {
					skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
					wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
				}
				break;
			case (POWROT):
				if (jestTrybAuto) {
					czyscTablice(tablicaCelow, ROZMIAR_MAPY);
					mapa[cel].rodzajWezla = ZNAK_WOLNE;
					wrocNaPoczatek(tablicaCelow, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
					zbadanoMape = false;
					osiagnietoCel = true;
				}
				break;
			case (AUTO_A):
				if (jestTrybAuto) {
					czyscTablice(tablicaCelow, ROZMIAR_MAPY);
					mapa[cel].rodzajWezla = ZNAK_WOLNE;
					wybierzCeleA(tablicaCelow, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
					zbadanoMape = false;
					osiagnietoCel = true;
				}
				break;
			case (AUTO_B):
				if (jestTrybAuto) {
					czyscTablice(tablicaCelow, ROZMIAR_MAPY);
					mapa[cel].rodzajWezla = ZNAK_WOLNE;
					wybierzCeleB(tablicaCelow, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
					zbadanoMape = false;
					osiagnietoCel = true;
				}
				break;
			case (AUTO_C):
				if (jestTrybAuto) {
					czyscTablice(tablicaCelow, ROZMIAR_MAPY);
					mapa[cel].rodzajWezla = ZNAK_WOLNE;
					wybierzCeleC(tablicaCelow, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
					zbadanoMape = false;
					osiagnietoCel = true;
				}
				break;
		}
	}
	//Dla trybu auto rozpoczęcie samodzielnego badania terenu
	if (jestTrybAuto) {
		//Ustawienie celu. Przed ustawieniem celu skanowanie otoczenia
		if (osiagnietoCel && !zbadanoMape) {
			cel = wyznaczCel(mapa, ROZMIAR_BOKU_MAPY, tablicaCelow, ROZMIAR_MAPY);
			if (cel != BRAK_WEZLA) {
				
				wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
		
				osiagnietoCel = false;
				moznaWykonacRuch = false;
				licznik = 0;
				
			} else {
				//zwrócono jako cel brak wezla wiec tablica celow nie zawiera nic
				//nalezy wyjsc z tego ifa i czekać na info o zaladowaniu nowych celow do tablicy
				osiagnietoCel = true;
				zbadanoMape = true;
			}
		} else
		//Wyznaczenie trasy
		if (!osiagnietoCel && !moznaWykonacRuch) {
			skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
			wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);

			pozycjaRobota = wykonajObrot90Prawo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
			skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
			wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
		
			ustalSasiadow(mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
			bool wyznaczonoTrase = wyznaczTrase(miejsceRobota, cel, mapa);
			if (wyznaczonoTrase) {
				mapa[cel].rodzajWezla = ZNAK_CEL; //bo gdy wykryjemy na  celu sciane a potem ona zniknie to chcemy nadal widziec cel
				wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);

				moznaWykonacRuch = true;
			} else {
				moznaWykonacRuch = false;
				osiagnietoCel = true;
				zbadanoMape = false;
				mapa[cel].rodzajWezla = ZNAK_WOLNE;
			}
		} else
		//Wykonanie jednego ruchu do celu
		if (!osiagnietoCel && moznaWykonacRuch) {
			bool wykrytoElement = false;
			wykrytoElement = skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
			wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);
			if (!wykrytoElement) {
				Robot robot = wykonajRuchDoCelu(mapa, miejsceRobota, pozycjaRobota, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, servo1, servo2);
			
				//robot stanął w miejscu celu
				if (robot.miejsceRobota == cel) {
					osiagnietoCel = true;
					zbadanoMape = false;
					licznik = 0;
				
				//robot pozostał na swoim miejscu
				} else if (miejsceRobota == robot.miejsceRobota) {
					moznaWykonacRuch = false;
			
					pozycjaRobota = wykonajObrot90Prawo(mapa, miejsceRobota, pozycjaRobota, servo1, servo2);
					skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY);
					wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);

					licznik++;
				
				//robot zmienił miejsce czyli ruch wykonany
				} else {
					licznik = 0;
				}
				miejsceRobota = robot.miejsceRobota;
				pozycjaRobota = robot.pozycjaRobota;
				wyslijStringJson(mapa, ROZMIAR_MAPY, JSON_MAPA);

			//gdy przed wykonaniem ruchu wykryto nowy element
			} else {
				moznaWykonacRuch = false;
			}
			//Po ilu ruchach bez przemieszczenia porzuca cel
			if (licznik == LICZBA_REZYGNACJI_CELU) {
				osiagnietoCel = true;
				zbadanoMape = false;
			}
		}
	}
}

