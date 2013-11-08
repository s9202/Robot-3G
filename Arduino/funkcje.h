#ifndef funkcje_h
#define funkcje_h

#include <Servo.h>
#include "Arduino.h"

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

const double IR_MIN = 4.0;
const double IR_MAX = 30.0;

const int ROZMIAR_BOKU_MAPY = 10;
const int ROZMIAR_MAPY = ROZMIAR_BOKU_MAPY * ROZMIAR_BOKU_MAPY;
const int PG_ROG = ROZMIAR_BOKU_MAPY - 1;
const int LD_ROG = ROZMIAR_MAPY - ROZMIAR_BOKU_MAPY;
const int PD_ROG = ROZMIAR_MAPY - 1;

//Biegunowa pozycja robota
const char PRZODEM_GORA = '8';
const char PRZODEM_PRAWO = '6';
const char PRZODEM_LEWO = '4';
const char PRZODEM_DOL = '2';

const char ZNAK_SCIANA = 'x';
const char ZNAK_WOLNE = ' ';
const char ZNAK_TRASA = '.';
const char ZNAK_CEL = 'C';

const int BRAK_WEZLA = -1;

const int IRLeft = 0;
const int IRRight = 1;
const int IRFront = 2;


typedef struct {
	int numerPoprzednika;
	int sasiadZLewej;
	int sasiadZPrawej;
	int sasiadZGory;
	int sasiadZDolu;
	char rodzajWezla;
} Wezel;

typedef struct Robot {
	int miejsceRobota;
	char pozycjaRobota;
} Robot;


//Funkcje ruchu skokowego przydatne dla kontroli automatycznej
int wykonajJedenRuchPrzod(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);
int wykonajJedenRuchTyl(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);
char wykonajObrot90Lewo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2);
char wykonajObrot90Prawo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2);

//Funkcje ruchu cigego przydatne dla kontroli zdalnej
void jedzProsto(Servo servo1, Servo servo2);
void skrecajWLewo(Servo servo1, Servo servo2);
void skrecajWPrawo(Servo servo1, Servo servo2);
void jedzDoTylu(Servo servo1, Servo servo2);
void zatrzymajRuch(Servo servo1, Servo servo2);

//Funkcje ruchu gowy
void zatrzymajGlowe(Servo servo3);
void glowaLewo(Servo servo3);
void glowaPrawo(Servo servo3);

//Funkcje odczytu danych z czujnikw otoczenia (Filip)
double skanujSensorPrzedni(int IRFront);
double skanujSensorLewy(int IRLeft);
double skanujSensorPrawy(int IRRight);

//Funkcje tworzce map (Maciej)
int sprawdzOdlegloscIZaznacz(double odleglosc, int miejsceRobota, char pozycjaCzujnika, Wezel tablica[], int rozmiarTablicy, int rozmiarBoku);
void inicjujMape(int rozmiarMapy, int rozmiarBoku, int miejsceRobota, char pozycjaRobota, Wezel mapa[]);
bool skanujZaznaczMape(int miejsceRobota, char pozycjaCzujnikaPrzod, Wezel tablica[], int rozmiarTablicy, int rozmiarBoku);
int zaznacz1Gora(int miejsceRobota, int rozmiarBoku);
int zaznacz2Gora(int miejsceRobota, int rozmiarBoku);
int zaznacz3Gora(int miejsceRobota, int rozmiarBoku);
int zaznacz1Prawo(int miejsceRobota, int rozmiarBoku);
int zaznacz2Prawo(int miejsceRobota, int rozmiarBoku);
int zaznacz3Prawo(int miejsceRobota, int rozmiarBoku);
int zaznacz1Lewo(int miejsceRobota, int rozmiarBoku);
int zaznacz2Lewo(int miejsceRobota, int rozmiarBoku);
int zaznacz3Lewo(int miejsceRobota, int rozmiarBoku);
int zaznacz1Dol(int miejsceRobota, int rozmiarBoku);
int zaznacz2Dol(int miejsceRobota, int rozmiarBoku);
int zaznacz3Dol(int miejsceRobota, int rozmiarBoku);
void czyscGora(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);
void czyscPrawo(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);
void czyscLewo(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);
void czyscDol(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);

//Funkcje samodzielnej jazdy robota
void ustalSasiadow(Wezel tablica[], int rozmiarTablicy, int rozmiarBoku);
void wyznaczTrase(int pPunktWejscia, int pPunktWyjscia, Wezel tablica[]);
bool wystapilWTablicy(int tablica[], int element, int rozmiarTablicy);
Robot wykonajRuchDoCelu(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);
int wyznaczCel(Wezel tablica[]);

//Funkcje pomocnicze
bool czyWezelJestSciana(Wezel tablica[], int badanyWezel, int rozmiarTablicy, int rozmiarBoku);

//void wyslijTablice(Wezel tablica[], int rozmiarTablicy);
void wyslijStringJson(Wezel tablica[], int rozmiarTablicy);

#endif