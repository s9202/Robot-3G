//#include <WProgram.h>
//#include<Arduino.h>
#include <Servo.h>

const char TRYB_MANUAL = '0';
const char TRYB_AUTO = '1';
const char JAZDA_PRZOD = 'w';
const char JAZDA_LEWO = 'a';
const char JAZDA_PRAWO = 'd';
const char JAZDA_TYL = 's';
const char JAZDA_PRZOD_SKOK = 'W';
const char JAZDA_LEWO_SKOK = 'A';
const char JAZDA_PRAWO_SKOK = 'D';
const char JAZDA_TYL_SKOK = 'S';
const char GLOWA_LEWO = 'q';
const char GLOWA_PRAWO = 'e';
const char JAZDA_STOP = '3';
const char GLOWA_STOP = '4';
const char SKAN_MAN = '5';
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
const double IR_MAX = 30.0;

//Biegunowa pozycja robota
const char PRZODEM_GORA = '8';
const char PRZODEM_PRAWO = '6';
const char PRZODEM_LEWO = '4';
const char PRZODEM_DOL = '2';

const char ZNAK_SCIANA = 'x';
const char ZNAK_WOLNE = 'o';

char pozycjaRobota = PRZODEM_GORA;

char mapa[ROZMIAR_MAPY];

char* wskKoniecMapy = &mapa[PD_ROG];

char* miejsceRobota = mapa;

char pozycjaCzujnikaLewy;
char pozycjaCzujnikaPrawy;


Servo servo1;
Servo servo2;
Servo servo3;

int IRLeft = 0;
int IRRight = 1;
int IRFront = 2;


bool jestTrybAuto = false;        //Tryb pracy robota (automatyczny = true)
char odebranyBajt;                        //Zmienna przechowująca bajt pobrany z SerialPortu

//------------------------------ Funkcje pomocnicze
//Funkcje samodzielnej jazdy robota
void badajTeren(char pozycjaRobota, char mapa[]);

//Funkcje ruchu skokowego przydatne dla kontroli automatycznej (Filip)
char* wykonajJedenRuchPrzod(char* miejsceRobota, char pozycjaRobota, int rozmiarBoku);
char* wykonajJedenRuchTyl(char* miejsceRobota, char pozycjaRobota, int rozmiarBoku);
char wykonajObrot90Lewo(char* miejsceRobota, char pozycjaRobota);
char wykonajObrot90Prawo(char* miejsceRobota, char pozycjaRobota);

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
char* sprawdzOdlegloscIZaznacz(double odleglosc, char* miejsceRobota, char pozycjaCzujnika);
void inicjujMape(int rozmiarMapy, int rozmiarBoku, char pozycjaRobota, char mapa[]);
void skanujZaznaczMape(char* miejsceRobota, char pozycjaCzujnikaPrzod, char mapa[], char* wskKoniecMapy);
char* zaznacz1Gora(char* miejsceRobota);
char* zaznacz2Gora(char* miejsceRobota);
char* zaznacz3Gora(char* miejsceRobota);
char* zaznacz1Prawo(char* miejsceRobota);
char* zaznacz2Prawo(char* miejsceRobota);
char* zaznacz3Prawo(char* miejsceRobota);
char* zaznacz1Lewo(char* miejsceRobota);
char* zaznacz2Lewo(char* miejsceRobota);
char* zaznacz3Lewo(char* miejsceRobota);
char* zaznacz1Dol(char* miejsceRobota);
char* zaznacz2Dol(char* miejsceRobota);
char* zaznacz3Dol(char* miejsceRobota);

//Funckje samodzielnego jeżdżenia (Maciej)
void badajTeren(char pozycjaRobota, char mapa[]);
void wykonajAutoRuch();

void wyslijTablice(char tablica[], int rozmiarTablicy);
void wyslijStringJson(char tablica[], int rozmiarTablicy);

//--------------------------- wybranie portów które będziemy używać
void setup() {
	pinMode(13, OUTPUT);                //Port13 to akurat jest wbudowana dioda
	pinMode(11, OUTPUT);  //dla piezo
	digitalWrite(13, LOW);        //port13
	Serial.begin(9600);                //Uruchomienie USB serialport z prędkością 9600

	//Tworzenie mapy początkowej
	inicjujMape(ROZMIAR_MAPY, ROZMIAR_BOKU_MAPY, pozycjaRobota, mapa);
	//Serial.println(mapa);
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
					jedzProsto();
				}
				break;
			case (JAZDA_LEWO):
				if (!jestTrybAuto) {
					skrecajWLewo();
				}
				break;
			case (JAZDA_PRAWO):
				if (!jestTrybAuto) {
					skrecajWPrawo();
				}
				break;
			case (JAZDA_TYL):
				if (!jestTrybAuto) {
					jedzDoTylu();
				}
				break;
			case (JAZDA_PRZOD_SKOK):
				if (!jestTrybAuto) {
					miejsceRobota = wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
				}
				break;
			case (JAZDA_LEWO_SKOK):
				if (!jestTrybAuto) {
					pozycjaRobota = wykonajObrot90Lewo(miejsceRobota, pozycjaRobota);
				}
				break;
			case (JAZDA_PRAWO_SKOK):
				if (!jestTrybAuto) {
					pozycjaRobota = wykonajObrot90Prawo(miejsceRobota, pozycjaRobota);
				}
				break;
			case (JAZDA_TYL_SKOK):
				if (!jestTrybAuto) {
					miejsceRobota = wykonajJedenRuchTyl(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
				}
				break;
			case (JAZDA_STOP):
				if (!jestTrybAuto) {
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
			case (SKAN_MAN):
				skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, wskKoniecMapy);
				//wyslijTablice(mapa, ROZMIAR_MAPY);
				wyslijStringJson(mapa, ROZMIAR_MAPY);
				break;
		}
		//Dla trybu auto rozpoczęcie samodzielnego badania terenu
		if (jestTrybAuto) {

			wykonajAutoRuch();

			skanujZaznaczMape(miejsceRobota, pozycjaRobota, mapa, wskKoniecMapy);



			//Serial.println(mapa);
		}
	}
}



//------------------------------ Implementacja funkcji pomocniczych

void jedzProsto() {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(45);
	servo2.write(135);
}
void skrecajWLewo() {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(45);
	servo2.write(45);
}
void skrecajWPrawo() {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(135);
	servo2.write(135);
}
void jedzDoTylu() {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(135);
	servo2.write(45);
}
void zatrzymajRuch() {
	servo1.detach();
	servo2.detach();
}
void zatrzymajGlowe() {
	servo3.detach();
}
void glowaLewo() {
	servo3.attach(3);
	servo3.write(45);
}
void glowaPrawo() {
	servo3.attach(3);
	servo3.write(135);
}
void badajTeren(char pozycjaRobota, char mapa[]) {
	wykonajObrot90Lewo(miejsceRobota, pozycjaRobota);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajObrot90Prawo(miejsceRobota, pozycjaRobota);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajObrot90Prawo(miejsceRobota, pozycjaRobota);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajObrot90Prawo(miejsceRobota, pozycjaRobota);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajObrot90Prawo(miejsceRobota, pozycjaRobota);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajObrot90Prawo(miejsceRobota, pozycjaRobota);
	wykonajJedenRuchPrzod(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
	wykonajJedenRuchTyl(miejsceRobota, pozycjaRobota, ROZMIAR_BOKU_MAPY);
}

char* wykonajJedenRuchPrzod(char* miejsceRobota, char pozycjaRobota, int rozmiarBoku) {

	//Tutaj kod serwomechanizmów
	
	//

	char* wskazGdzieZaznaczyc = miejsceRobota;
	switch (pozycjaRobota) {
		case (PRZODEM_GORA):	wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - rozmiarBoku; break;
		case (PRZODEM_LEWO):	wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - 1; break;
		case (PRZODEM_PRAWO):	wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + 1; break;
		case (PRZODEM_DOL):		wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + rozmiarBoku; break;
	}
	if (wskazGdzieZaznaczyc != miejsceRobota && *wskazGdzieZaznaczyc != ZNAK_SCIANA && wskazGdzieZaznaczyc >= mapa && wskazGdzieZaznaczyc <= wskKoniecMapy && *wskazGdzieZaznaczyc != 'X' && *wskazGdzieZaznaczyc != 'P' && *wskazGdzieZaznaczyc != 'L') {
		*miejsceRobota = ZNAK_WOLNE;
		miejsceRobota = wskazGdzieZaznaczyc;
		*miejsceRobota = pozycjaRobota;
	}
	return miejsceRobota;
}
char* wykonajJedenRuchTyl(char* miejsceRobota, char pozycjaRobota, int rozmiarBoku) {

	//Tutaj kod serwomechanizmów
	
	//

	char* wskazGdzieZaznaczyc = miejsceRobota;
	switch (pozycjaRobota) {
		case (PRZODEM_GORA):	wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + rozmiarBoku; break;
		case (PRZODEM_LEWO):	wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + 1; break;
		case (PRZODEM_PRAWO):	wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - 1; break;
		case (PRZODEM_DOL):		wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - rozmiarBoku; break;
	}
	if (wskazGdzieZaznaczyc != miejsceRobota && *wskazGdzieZaznaczyc != ZNAK_SCIANA && wskazGdzieZaznaczyc >= mapa && wskazGdzieZaznaczyc <= wskKoniecMapy && *wskazGdzieZaznaczyc != 'X' && *wskazGdzieZaznaczyc != 'P' && *wskazGdzieZaznaczyc != 'L') {
		*miejsceRobota = ZNAK_WOLNE;
		miejsceRobota = wskazGdzieZaznaczyc;
		*miejsceRobota = pozycjaRobota;
	}
	return miejsceRobota;
}
char wykonajObrot90Lewo(char* miejsceRobota, char pozycjaRobota) {

	//Tutaj kod serwomechanizmów
	
	//

	//Zmiana pozycji robota
	switch (pozycjaRobota) {
		case (PRZODEM_GORA):	pozycjaRobota = PRZODEM_LEWO; break;
		case (PRZODEM_LEWO):	pozycjaRobota = PRZODEM_DOL; break;
		case (PRZODEM_PRAWO):	pozycjaRobota = PRZODEM_GORA; break;
		case (PRZODEM_DOL):		pozycjaRobota = PRZODEM_PRAWO; break;
	}
	*miejsceRobota = pozycjaRobota;
	return pozycjaRobota;
}
char wykonajObrot90Prawo(char* miejsceRobota, char pozycjaRobota) {

	//Tutaj kod serwomechanizmów
	
	//

	//Zmiana pozycji robota
	switch (pozycjaRobota) {
		case (PRZODEM_GORA):	pozycjaRobota = PRZODEM_PRAWO; break;
		case (PRZODEM_LEWO):	pozycjaRobota = PRZODEM_GORA; break;
		case (PRZODEM_PRAWO):	pozycjaRobota = PRZODEM_DOL; break;
		case (PRZODEM_DOL):		pozycjaRobota = PRZODEM_LEWO; break;
	}
	*miejsceRobota = pozycjaRobota;
	return pozycjaRobota;
}

double skanujSensorPrzedni() {
	double volts = analogRead(IRFront)*0.004692082;   
	double odlegloscZmierzona =(14.925-(1.5*volts))/(volts+0.05);  
	return odlegloscZmierzona;
}
double skanujSensorLewy() {
	double volts = analogRead(IRLeft)*0.004692082;   
	double odlegloscZmierzona =(14.925-(1.5*volts))/(volts+0.05);  
	return odlegloscZmierzona;
}
double skanujSensorPrawy() {
	double volts = analogRead(IRRight)*0.004692082;   
	double odlegloscZmierzona =(14.925-(1.5*volts))/(volts+0.05);  
	return odlegloscZmierzona;
}

char* sprawdzOdlegloscIZaznacz(double odleglosc, char* miejsceRobota, char pozycjaCzujnika) {

	if (odleglosc > IR_MIN && odleglosc <= 20.0) {
	beep(100);
		switch (pozycjaCzujnika) {
			case (PRZODEM_GORA): return zaznacz1Gora(miejsceRobota);
			case (PRZODEM_PRAWO): return zaznacz1Prawo(miejsceRobota);
			case (PRZODEM_LEWO): return zaznacz1Lewo(miejsceRobota);
			case (PRZODEM_DOL): return zaznacz1Dol(miejsceRobota);
		}
	} else if (odleglosc > 20.0 && odleglosc <= 40.0) {
		switch (pozycjaCzujnika) {
			case (PRZODEM_GORA): return zaznacz2Gora(miejsceRobota);
			case (PRZODEM_PRAWO): return zaznacz2Prawo(miejsceRobota);
			case (PRZODEM_LEWO): return zaznacz2Lewo(miejsceRobota);
			case (PRZODEM_DOL): return zaznacz2Dol(miejsceRobota);
		}
	/*                
	} else if (odleglosc > 20.0 && odleglosc <= 30.0) {
		switch (pozycjaCzujnika) {
			case (PRZODEM_GORA): return zaznacz3Gora(miejsceRobota);
			case (PRZODEM_PRAWO): return zaznacz3Prawo(miejsceRobota);
			case (PRZODEM_LEWO): return zaznacz3Lewo(miejsceRobota);
			case (PRZODEM_DOL): return zaznacz3Dol(miejsceRobota);
		}
	*/                
	} else {
		return miejsceRobota;
	}
}

void wykonajAutoRuch() {
	wykonajObrot90Lewo(miejsceRobota, pozycjaRobota);
}
void inicjujMape(int rozmiarMapy, int rozmiarBoku, char pozycjaRobota, char mapa[]) {
	for (int i=0; i<rozmiarBoku; i++) {
		mapa[i] = ZNAK_SCIANA;
	}
	for (int i=rozmiarBoku; i<rozmiarMapy-rozmiarBoku; i++) {
		if (i%10 == 0 || i%10 == 9) {
			mapa[i] = ZNAK_SCIANA;
		} else {
			mapa[i] = ZNAK_WOLNE;
		}
	}
	for (int i=rozmiarMapy-rozmiarBoku; i<rozmiarMapy; i++) {
		mapa[i] = ZNAK_SCIANA;
	}
	mapa[(rozmiarMapy + rozmiarBoku) /2] = pozycjaRobota;
	miejsceRobota = &mapa[(rozmiarMapy + rozmiarBoku) /2];
}
void skanujZaznaczMape(char* miejsceRobota, char pozycjaCzujnikaPrzod, char mapa[], char* wskKoniecMapy){
	double s1 = skanujSensorPrzedni();
	double s2 = skanujSensorLewy();
	double s3 = skanujSensorPrawy();

	//Ustalenie pozycji w jakiej są czujniki
	switch (pozycjaCzujnikaPrzod) {
		case (PRZODEM_GORA):	pozycjaCzujnikaLewy = PRZODEM_LEWO;
								pozycjaCzujnikaPrawy = PRZODEM_PRAWO;
								break;
		case (PRZODEM_LEWO):	pozycjaCzujnikaLewy = PRZODEM_DOL;
								pozycjaCzujnikaPrawy = PRZODEM_GORA;
								break;
		case (PRZODEM_PRAWO):	pozycjaCzujnikaLewy = PRZODEM_GORA;
								pozycjaCzujnikaPrawy = PRZODEM_DOL;
								break;
		case (PRZODEM_DOL):		pozycjaCzujnikaLewy = PRZODEM_PRAWO;
								pozycjaCzujnikaPrawy = PRZODEM_LEWO;
								break;
	}

	char* wskazGdzieZaznaczycPrzod = sprawdzOdlegloscIZaznacz(s1, miejsceRobota, pozycjaCzujnikaPrzod);
	char* wskazGdzieZaznaczycLewo = sprawdzOdlegloscIZaznacz(s2, miejsceRobota, pozycjaCzujnikaLewy);
	char* wskazGdzieZaznaczycPrawo = sprawdzOdlegloscIZaznacz(s3, miejsceRobota, pozycjaCzujnikaPrawy);

	if (wskazGdzieZaznaczycPrzod != miejsceRobota && *wskazGdzieZaznaczycPrzod != ZNAK_SCIANA && wskazGdzieZaznaczycPrzod >= mapa && wskazGdzieZaznaczycPrzod <= wskKoniecMapy) {
		*wskazGdzieZaznaczycPrzod = 'X';
	}
	if (wskazGdzieZaznaczycLewo != miejsceRobota && *wskazGdzieZaznaczycLewo != ZNAK_SCIANA && wskazGdzieZaznaczycLewo >= mapa && wskazGdzieZaznaczycLewo <= wskKoniecMapy) {
		*wskazGdzieZaznaczycLewo = 'L';
	}
	if (wskazGdzieZaznaczycPrawo != miejsceRobota && *wskazGdzieZaznaczycPrawo != ZNAK_SCIANA && wskazGdzieZaznaczycPrawo >= mapa && wskazGdzieZaznaczycPrawo <= wskKoniecMapy) {
		*wskazGdzieZaznaczycPrawo = 'P';
	}
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

void wyslijTablice(char tablica[], int rozmiarTablicy) {
	for (int i=0; i<rozmiarTablicy; i++) {
		Serial.print(tablica[i]);
	}
	Serial.println();
}
void wyslijStringJson(char tablica[], int rozmiarTablicy) {
	Serial.println("{ \"mapa\":\"" +String(tablica)+ "\" }");
}

void beep(unsigned char delayms){
	analogWrite(11, 20);     
	delay(delayms);         
	analogWrite(11, 0);   
	delay(delayms);         
}  
