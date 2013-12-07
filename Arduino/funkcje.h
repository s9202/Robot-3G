#ifndef funkcje_h
#define funkcje_h

#include <Servo.h>
#include "Arduino.h"

//Znaki oczekiwane z portu szeregowego
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
const char POWROT = '6';
const char AUTO_A = '7';
const char AUTO_B = '8';
const char AUTO_C = '9';

//Okreslenie rozmiaru boku mapy i wyliczenia mapy
const int ROZMIAR_BOKU_MAPY = 10;
const int ROZMIAR_MAPY = ROZMIAR_BOKU_MAPY * ROZMIAR_BOKU_MAPY;
const int PG_ROG = ROZMIAR_BOKU_MAPY - 1;
const int LD_ROG = ROZMIAR_MAPY - ROZMIAR_BOKU_MAPY;
const int PD_ROG = ROZMIAR_MAPY - 1;

//Biegunowa pozycja robota widoczna na mapie
const char PRZODEM_GORA = '8';
const char PRZODEM_PRAWO = '6';
const char PRZODEM_LEWO = '4';
const char PRZODEM_DOL = '2';

//Znaki wykorzystywane do wyswietlania mapy
const char ZNAK_SCIANA = 'x';
const char ZNAK_WOLNE = '0';
const char ZNAK_TRASA = '.';
const char ZNAK_CEL = 'C';

//Przyjete oznaczenie braku wezla
const int BRAK_WEZLA = -1;

//Wartosci okreslajace zasieg wykorzystywanych czujnikow
const double IR_MIN = 4.0;
const double IR_MAX = 30.0;

//Piny odpowiadajace kazdemu czujnikowi
const int IRLeft = 0;
const int IRRight = 1;
const int IRFront = 2;

//Licznik okreslajacy dopuszczalna liczbe nieudanego znalezienia drogi do celu
const int LICZBA_REZYGNACJI_CELU = 4;

/* Rodzaje wysylanego JSONA
 *
 * JSON_MAPA zostaje wyslana tylko mapa
 * JSON_ZADANIE zostaje wyslana tylko odpowiedz o wykonaniu zadania ("1")
 * JSON_MAPA_ZADANIE zostaje wyslana mapa i wykonanie zadania ("1")
 */
const int JSON_MAPA = 0;
const int JSON_ZADANIE = 1;
const int JSON_MAPA_ZADANIE = 2;


//Struktura wezla z jakich sklada sie mapa
typedef struct {
	int numerPoprzednika;
	int sasiadZLewej;
	int sasiadZPrawej;
	int sasiadZGory;
	int sasiadZDolu;
	char rodzajWezla;
} Wezel;

//Struktura opisujaca pozycje i miejsce robota
typedef struct Robot {
	int miejsceRobota;
	char pozycjaRobota;
} Robot;

//Funkcje ruchu ciglego przydatne dla kontroli zdalnej
void jedzProsto(Servo servo1, Servo servo2);
void skrecajWLewo(Servo servo1, Servo servo2);
void skrecajWPrawo(Servo servo1, Servo servo2);
void jedzDoTylu(Servo servo1, Servo servo2);
void zatrzymajRuch(Servo servo1, Servo servo2);

//Funkcje ruchu glowy
void zatrzymajGlowe(Servo servo3);
void glowaLewo(Servo servo3);
void glowaPrawo(Servo servo3);

//Funkcja odczytu danych z czujnikow otoczenia
double skanujSensor(int iR);

//Funkcja emitowania sygnalu dzwiekowego
void beep(unsigned char delayms);

//Funkcje ruchu skokowego przydatne dla kontroli automatycznej
int wykonajJedenRuchPrzod(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);
int wykonajJedenRuchTyl(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);
char wykonajObrot90Lewo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2);
char wykonajObrot90Prawo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2);

//Funkcje tworzace mape
int inicjujMape(int rozmiarTablicy, int rozmiarBoku, int miejsceRobota, char pozycjaRobota, Wezel mapa[]);
int sprawdzOdlegloscIZaznacz(double odleglosc, int miejsceRobota, char pozycjaCzujnika, Wezel tablica[], int rozmiarTablicy, int rozmiarBoku);
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
bool wyznaczTrase(int pPunktWejscia, int pPunktWyjscia, Wezel tablica[]);
Robot wykonajRuchDoCelu(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);
void wybierzCeleA(int tablicaCelowNowych[], int rozmiarTablicy, int rozmiarBoku);
void wybierzCeleB(int tablicaCelowNowych[], int rozmiarTablicy, int rozmiarBoku);
void wybierzCeleC(int tablicaCelowNowych[], int rozmiarTablicy, int rozmiarBoku);
int wyznaczCel(Wezel tablica[], int rozmiarBoku, int tablicaCelowNowych[], int rozmiarTablicy);
void wrocNaPoczatek(int tablicaCelow[], int rozmiarTablicy, int rozmiarBoku);

//Funkcje pomocnicze
bool wystapilWTablicy(int tablica[], int element, int rozmiarTablicy);
bool czyWezelJestSciana(Wezel tablica[], int badanyWezel, int rozmiarTablicy, int rozmiarBoku);
void czyscTablice(int tablica[], int rozmiarTablicy);

//Pisanie na port szeregowy
void wyslijStringJson(Wezel tablica[], int rozmiarTablicy, int zestawDanych);

#endif
