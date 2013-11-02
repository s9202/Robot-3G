//#include <WProgram.h>
//#include<Arduino.h>
#include <Servo.h>

const char TRYB_MANUAL = '0';
const char TRYB_AUTO = '1';
const char JAZDA_PRZOD = 'w';
const char JAZDA_LEWO = 'a';
const char JAZDA_PRAWO = 'd';
const char JAZDA_TYL = 's';
const char GLOWA_LEWO = 'q';
const char GLOWA_PRAWO = 'e';
const char JAZDA_STOP = '3';
const char GLOWA_STOP = '4';
//const char JAZDA_STOP1 = '2';
//const char JAZDA_STOP2 = '4';
//const char JAZDA_STOP3 = '6';
//const char JAZDA_STOP4 = '0';
//const char GLOWA_STOP1 = 'a';
//const char GLOWA_STOP2 = 'b';

const int ROZMIAR_BOKU_MAPY = 10;
const int ROZMIAR_MAPY = ROZMIAR_BOKU_MAPY * ROZMIAR_BOKU_MAPY;
const int PG_ROG = ROZMIAR_BOKU_MAPY - 1;
const int LD_ROG = ROZMIAR_MAPY - ROZMIAR_BOKU_MAPY;
const int PD_ROG = ROZMIAR_MAPY - 1;

const double IR_MIN = 4.0;
const double IR_MAX = 40.0;

//Biegunowa pozycja robota
const char PRZODEM_GORA = '8';
const char PRZODEM_PRAWO = '6';
const char PRZODEM_LEWO = '4';
const char PRZODEM_DOL = '2';

char pozycjaRobota = PRZODEM_GORA;

char mapa[ROZMIAR_MAPY] = {
	'x','x','x','x','x','x','x','x','x','x',
	'x','o','o','o','o','o','o','o','o','x',
	'x','o','o','o','o','o','o','o','o','x',
	'x','o','o','o','o','o','o','o','o','x',
	'x','o','o','o','o','o','o','o','o','x',
	'x','o','o','o','o',pozycjaRobota,'o','o','o','x',
	'x','o','o','o','o','o','o','o','o','x',
	'x','o','o','o','o','o','o','o','o','x',
	'x','o','o','o','o','o','o','o','o','x',
	'x','x','x','x','x','x','x','x','x','x'
};

char* miesceRobota = &mapa[56];


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
void zatrzymajRuch();

//Funkcje ruchu głowy
void zatrzymajGlowe();
void glowaLewo();
void glowaPrawo();

//Funkcje odczytu danych z czujników otoczenia (Filip)
double skanujSensorPrzedni();
double skanujSensorLewy();
double skanujSensorPrawy();

//Funkcje tworzące mapę (Maciej)
void tworzMape(char mapa[]);
char* zaznacz1Gora(char* miejsceRobota);
char* zaznacz2Gora(char* miejsceRobota);
char* zaznacz3Gora(char* miejsceRobota);
char* zaznacz1Prawo(char* miejsceRobota);
char* zaznacz2Prawo(char* miejsceRobota);
char* zaznacz3Prawo(char* miejsceRobota);
char* zaznacz1Lewo(char* miejsceRobota);
char* zaznacz2Lewo(char* miejsceRobota);
char* zaznacz3Lewo(char* miejsceRobota);

//Funckje samodzielnego jeżdżenia (Maciej)
void badajTeren(char mapa[]);
void ustaw();
char* sprawdzOdlegloscIZaznacz(double odleglosc, char* miejsceRobota);
void wykonajAutoRuch();

//--------------------------- wybranie portów które będziemy używać
void setup() {
	pinMode(13, OUTPUT);		//Port13 to akurat jest wbudowana dioda
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
			case (JAZDA_STOP):
				if (!autoModeOn) {
					zatrzymajRuch();
				}
				break;
			case (GLOWA_LEWO):
				glowaLewo();
				break;
			case (GLOWA_PRAWO):
				glowaPrawo();
				break;
			case (GLOWA_STOP):
				zatrzymajGlowe();
				break;
		}
		//Dla trybu auto rozpoczęcie samodzielnego badania terenu
		if (autoModeOn) {
			wykonajAutoRuch();
			double s1 = skanujSensorPrzedni();
			double s2 = skanujSensorLewy();
			double s3 = skanujSensorPrawy();
			

			char* wskazGdzieZaznaczycPrzod = sprawdzOdlegloscIZaznacz(s1, miesceRobota);
			char* wskazGdzieZaznaczycPrawo = sprawdzOdlegloscIZaznacz(s2, miesceRobota);
			char* wskazGdzieZaznaczycLewo = sprawdzOdlegloscIZaznacz(s3, miesceRobota);
			
			if (wskazGdzieZaznaczycPrzod != miesceRobota) {
				*wskazGdzieZaznaczycPrzod = pozycjaRobota;
			}
			if (wskazGdzieZaznaczycPrawo != miesceRobota) {
				*wskazGdzieZaznaczycPrawo = pozycjaRobota;
			}
			if (wskazGdzieZaznaczycLewo != miesceRobota) {
				*wskazGdzieZaznaczycLewo = pozycjaRobota;
			}
			
			
  			Serial.println(mapa);
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
void zatrzymajRuch() {
	servo1.detach();
	servo2.detach();
}
void zatrzymajGlowe() {
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
void badajTeren(char mapa[]) {
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

	tworzMape(mapa);
}

void wykonajJedenRuchPrzod() {
}
void wykonajJedenRuchTyl() {
}
void wykonajObrot90Lewo() {
}
void wykonajObrot90Prawo() {
}

double skanujSensorPrzedni() {
	double odlegloscZmierzona;
	return odlegloscZmierzona;
}
double skanujSensorLewy() {
	double odlegloscZmierzona;
	return odlegloscZmierzona;
}
double skanujSensorPrawy() {
	double odlegloscZmierzona;
	return odlegloscZmierzona;
}

void tworzMape(char mapa[]){

}

void ustaw(){
}

char* sprawdzOdlegloscIZaznacz(double odleglosc, char* miejsceRobota) {

	if (odleglosc > IR_MIN && odleglosc <= 10.0) {
		switch (pozycjaRobota) {
			case (PRZODEM_GORA): return zaznacz1Gora(miejsceRobota);
			case (PRZODEM_PRAWO): return zaznacz1Prawo(miejsceRobota);
			case (PRZODEM_LEWO): return zaznacz1Lewo(miejsceRobota);
			case (PRZODEM_DOL): return zaznacz1Dol(miejsceRobota);
		}
	} else if (odleglosc > 10.0 && odleglosc <= 20.0) {
		switch (pozycjaRobota) {
			case (PRZODEM_GORA): return zaznacz2Gora(miejsceRobota);
			case (PRZODEM_PRAWO): return zaznacz2Prawo(miejsceRobota);
			case (PRZODEM_LEWO): return zaznacz2Lewo(miejsceRobota);
			case (PRZODEM_DOL): return zaznacz2Dol(miejsceRobota);
		}
		
	} else if (odleglosc > 20.0 && odleglosc <= 30.0) {
		switch (pozycjaRobota) {
			case (PRZODEM_GORA): return zaznacz3Gora(miejsceRobota);
			case (PRZODEM_PRAWO): return zaznacz3Prawo(miejsceRobota);
			case (PRZODEM_LEWO): return zaznacz3Lewo(miejsceRobota);
			case (PRZODEM_DOL): return zaznacz3Dol(miejsceRobota);
		}
		
	} else {
		return miejsceRobota;
	}
	
	
	
}

void wykonajAutoRuch() {
	wykonajObrot90Lewo();
}

char* zaznacz1Gora(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota - ROZMIAR_BOKU_MAPY;
	return miejsceZaznaczenia;
}
char* zaznacz2Gora(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota - (2 * ROZMIAR_BOKU_MAPY);
	return miejsceZaznaczenia;
}
char* zaznacz3Gora(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota - (3 * ROZMIAR_BOKU_MAPY);
	return miejsceZaznaczenia;
}
char* zaznacz1Lewo(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota - 1;
	return miejsceZaznaczenia;
}
char* zaznacz2Lewo(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota - 2;
	return miejsceZaznaczenia;
}
char* zaznacz3Lewo(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota - 3;
	return miejsceZaznaczenia;
}
char* zaznacz1Prawo(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota + 1;
	return miejsceZaznaczenia;
}
char* zaznacz2Prawo(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota + 2;
	return miejsceZaznaczenia;
}
char* zaznacz3Prawo(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota + 3;
	return miejsceZaznaczenia;
}
char* zaznacz1Dol(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota + ROZMIAR_BOKU_MAPY;
	return miejsceZaznaczenia;
}
char* zaznacz2Dol(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota + (2 * ROZMIAR_BOKU_MAPY);
	return miejsceZaznaczenia;
}
char* zaznacz3Dol(char* miejsceRobota) {
	char* miejsceZaznaczenia = miejsceRobota + (3 * ROZMIAR_BOKU_MAPY);
	return miejsceZaznaczenia;
}

