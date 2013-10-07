#include <Servo.h>

const char TRYB_MANUAL = '0';
const char TRYB_AUTO = '1';
const char JAZDA_PRZOD = '2';
const char JAZDA_LEWO = '3';
const char JAZDA_PRAWO = '4';
const char JAZDA_TYL = '5';
const char JAZDA_STOP = '6';

Servo servo1;
Servo servo2;

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
	pinMode(13, OUPUT);		//Port13 to akurat jest wbudowana dioda
	digitalWrite(13, LOW);	//port13
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
			case (TRYB_MANUAL):
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
			case (JAZDA_STOP):
				if (!autoModeOn) {
					zatrzymaj();
				}
				break;
		}
		//Dla trybu auto rozpoczęcie samodzielnego badania terenu
		if (autoModeOn) {
			badajTeren();
			Serial.println(tworzMape());
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





























