//#include <WProgram.h>
//#include<Arduino.h>
#include <Servo.h>

const char TRYB_MANUAL = 'o';
const char TRYB_AUTO = 'c';
const char JAZDA_PRZOD = '1';
const char JAZDA_LEWO = '3';
const char JAZDA_PRAWO = '7';
const char JAZDA_TYL = '5';
const char JAZDA_STOP1 = '2';
const char JAZDA_STOP2 = '4';
const char JAZDA_STOP3 = '6';
const char JAZDA_STOP4 = '0';
const char GLOWA_LEWO = 'l';
const char GLOWA_PRAWO = 'e';
const char GLOWA_STOP1 = 'a';
const char GLOWA_STOP2 = 'b';

Servo servo1;
Servo servo2;
Servo servo3;

bool autoModeOn = false;	//Tryb pracy robota (automatyczny = true)
char recievedByte;			//Zmienna przechowująca bajt pobrany z SerialPortu

//------------------------------ Funkcje pomocnicze

//Funkcje ruchu skokowego przydatne dla kontroli automatycznej (Filip)
void wykonajJedenRuchPrzod();
void wykonajJedenRuchTyl();
void wykonajObrot90Lewo();
void wykonajObrot90Prawo();

//Funkcje ruchu ciągłego przydatne dla kontroli zdalnej
void jedzProsto();
void skrecajWLewo();
void skrecajWPrawo();
void jedzDoTylu();

//Funkcje ruchu głowy
void zatrzymajLeb();
void glowaLewo();
void glowaPrawo();

//Funkcje odczytu danych z czujników otoczenia (Filip)
void skanujSensor1();
void skanujSensor2();

//Funkcje tworzące mapę (Maciej)
char tworzMape();

//Funckje samodzielnego jeżdżenia (Maciej)
void badajTeren();
void ustaw();

//--------------------------- wybranie portów które będziemy używać
void setup() {
//	pinMode(13, OUPUT);		//Port13 to akurat jest wbudowana dioda
//	digitalWrite(13, LOW);	//port13
	Serial.begin(9600);		//Uruchomienie USB serialport z prędkością 9600
}

//------------------------------ Główna pętla z programem

void loop() {

	//Początek pętli zawsze badanie bajtu na serialport
	if (Serial.available() > 0) {
		recievedByte = Serial.read();
		
		switch (recievedByte) {
			case (TRYB_MANUAL):
				autoModeOn = false;
				break;
			case (TRYB_AUTO):
				autoModeOn = true;
				break;
			case (JAZDA_PRZOD):
				if (!autoModeOn) {
					jedzProsto();
				}
				break;
			case (JAZDA_LEWO):
				if (!autoModeOn) {
					skrecajWLewo();
				}
				break;
			case (JAZDA_PRAWO):
				if (!autoModeOn) {
					skrecajWPrawo();
				}
				break;
			case (JAZDA_TYL):
				if (!autoModeOn) {
					jedzDoTylu();
				}
				break;
			case (JAZDA_STOP1):
				if (!autoModeOn) {
					zatrzymaj();
				}
				break;
			case (JAZDA_STOP2):
				if (!autoModeOn) {
					zatrzymaj();
				}
				break;
			case (JAZDA_STOP3):
				if (!autoModeOn) {
					zatrzymaj();
				}
				break;
			case (JAZDA_STOP4):
				if (!autoModeOn) {
					zatrzymaj();
				}
				break;
			case (GLOWA_LEWO):
				glowaLewo();
				break;
			case (GLOWA_PRAWO):
				glowaPrawo();
				break;
			case (GLOWA_STOP1):
				zatrzymajLeb();
				break;
			case (GLOWA_STOP2):
				zatrzymajLeb();
				break;
		}
		//Dla trybu auto rozpoczęcie samodzielnego badania terenu
		if (autoModeOn) {
			badajTeren();
//			Serial.println(tworzMape());
		}

	}
}



//------------------------------ Implementacja funkcji pomocniczych

void jedzProsto() {
	servo1.attach(14);
	servo2.attach(15);
	servo1.write(135);
	servo2.write(45);
}
void skrecajWLewo() {
	servo1.attach(14);
	servo2.attach(15);
	servo1.write(80);
	servo2.write(80);
}
void skrecajWPrawo() {
	servo1.attach(14);
	servo2.attach(15);
	servo1.write(100);
	servo2.write(100);
}
void jedzDoTylu() {
	servo1.attach(14);
	servo2.attach(15);
	servo1.write(45);
	servo2.write(135);
}
void zatrzymaj() {
	servo1.detach();
	servo2.detach();
}
void zatrzymajLeb() {
	servo3.detach();
}
void glowaLewo() {
	servo3.attach(16);
	servo3.write(45);
}
void glowaPrawo() {
	servo3.attach(16);
	servo3.write(135);
}
void badajTeren() {
	wykonajObrot90Lewo();
	wykonajJedenRuchPrzod();
	wykonajObrot90Prawo();
	wykonajJedenRuchPrzod();
	wykonajJedenRuchPrzod();
	wykonajObrot90Prawo();
	wykonajJedenRuchPrzod();
	wykonajJedenRuchPrzod();
	wykonajObrot90Prawo();
	wykonajJedenRuchPrzod();
	wykonajJedenRuchPrzod();
	wykonajObrot90Prawo();
	wykonajJedenRuchPrzod();
	wykonajObrot90Prawo();
	wykonajJedenRuchPrzod();
	wykonajJedenRuchTyl();
}

void wykonajJedenRuchPrzod(){
}
void wykonajJedenRuchTyl(){
}
void wykonajObrot90Lewo(){
}
void wykonajObrot90Prawo(){
}

void skanujSensor1(){
}
void skanujSensor2(){
}

char tworzMape(){
	char mapa;
	return mapa;
}

void ustaw(){
}
