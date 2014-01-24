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
const char WSP_ROBOT = '-';
const char WSP_MUR = '=';
const char WSP_WOLNE = '_';

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
const char ZNAK_MUR = 'X';
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


/*
 *Uruchomienie obu serwomechanizmów do jazdy w kierunku do przodu
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *
*/
void jedzProsto(Servo servo1, Servo servo2);

/*
 *Uruchomienie obu serwomechanizmów do obrotu w lewo
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *
*/
void skrecajWLewo(Servo servo1, Servo servo2);

/*
 *Uruchomienie obu serwomechanizmów do obrotu w prawo
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *
*/
void skrecajWPrawo(Servo servo1, Servo servo2);

/*
 *Uruchomienie obu serwomechanizmów do jazdy w kierunku do tyłu
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *
*/
void jedzDoTylu(Servo servo1, Servo servo2);

/*
 *Zatrzymanie ruchu serwomechanizmów kół
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *
*/
void zatrzymajRuch(Servo servo1, Servo servo2);

/*
 *Zatrzymanie ruchu serwomechanizmu głowy robota
 *@param servo3 serwomechanizm statywu głowy
 *
*/
void zatrzymajGlowe(Servo servo3);

/*
 *Uruchomienie serwomechanizmu do obrotu głowy w lewo
 *@param servo3 serwomechanizm statywu głowy
 *
*/
void glowaLewo(Servo servo3);

/*
 *Uruchomienie serwomechanizmu do obrotu głowy w prawo
 *@param servo3 serwomechanizm statywu głowy
 *
*/
void glowaPrawo(Servo servo3);

/*
 *Odczytuje dane z pinu związanego z czujnikiem odległości
 *@param iR pin wybrany do odczytu danych
 *@return obliczona wartość z czujnika wyrażona w centymetrach
 *
*/
double skanujSensor(int iR);

/*
 *Włączenie sygnału dźwiękowego na odpowiednio długi czas
 *@param delayms czas wyrażony w milisekundach przez jaki słyszalny jest dźwięk
*/
void beep(unsigned char delayms);

//Funkcje ruchu skokowego przydatne dla kontroli automatycznej--------------------------------------------------

/*
 *Przemieszczenie robota o jedno pole w kierunku do przodu, zgodnym z jego obecnym kierunkiem
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaRobota kierunek, w którym ustawiony jest robot względem krawędzi tablicy mapy
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *@return miejsce robota na mapie po wykonaniu funkcji
 *
*/
int wykonajJedenRuchPrzod(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);

/*
 *Przemieszczenie robota o jedno pole w kierunku do tyłu, zgodnym z jego obecnym kierunkiem
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaRobota kierunek, w którym ustawiony jest robot względem krawędzi tablicy mapy
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *@return miejsce robota na mapie po wykonaniu funkcji
 *
*/
int wykonajJedenRuchTyl(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);
/*
 *Obrócenie robota o 90 stopni w prawo w odniesieniu do jego obecnej pozycji
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaRobota kierunek, w którym ustawiony jest robot względem krawędzi tablicy mapy
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *@return miejsce robota na mapie po wykonaniu funkcji
 *
*/
char wykonajObrot90Lewo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2);

/*
 *Obrócenie robota o 90 stopni w prawo w odniesieniu do jego obecnej pozycji
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaRobota kierunek, w którym ustawiony jest robot względem krawędzi tablicy mapy
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *@return aktualna pozycja robota
 *
*/
char wykonajObrot90Prawo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2);

//Funkcje tworzace mape-----------------------------------------------------------------------------------

/*
 *Inicjacja podanej mapy pustymi wartościami
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaRobota kierunek, w którym ustawiony jest robot względem krawędzi tablicy mapy
 *@param tablica[] aktualna mapa otoczenia
 *@return aktualna pozycja robota
 *
*/
int inicjujMape(int rozmiarTablicy, int rozmiarBoku, int miejsceRobota, char pozycjaRobota, Wezel tablica[]);

/*
 *Bada pole na mapie na które wskazuje wartość z czujnika odległości w odniesieniu do pozycji robota.
 *W zależności od efektu wskazuje miejsce do wstawienia obiektu lub usuwa obiekt z tablicy mapy
 *@param odleglosc wartość odczytana z czujnika odległości
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaCzujnika kierunek, w którym ustawiony jest czujnik względem krawędzi tablicy mapy
 *@param tablica[] aktualna mapa otoczenia
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks pola mapy do zmiany
 *
*/
int sprawdzOdlegloscIZaznacz(double odleglosc, int miejsceRobota, char pozycjaCzujnika, Wezel tablica[], int rozmiarTablicy, int rozmiarBoku);

/*
 *Wykonuje odczyt wszystkimi czujnikami odległości i odpowiednio edytuje pola mapy w zasięgu czujników
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaCzujnikaPrzod kierunek, w którym ustawiony jest czujnik przedni względem krawędzi tablicy mapy
 *@param tablica[] aktualna mapa otoczenia
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return informacja czy w wyniku działania funkcji pojawił się nowy obiekt na mapie
 *
*/
bool skanujZaznaczMape(int miejsceRobota, char pozycjaCzujnikaPrzod, Wezel tablica[], int rozmiarTablicy, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o jedno pole nad robotem
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz1Gora(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o dwa pola nad robotem
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz2Gora(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o trzy pola nad robotem
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz3Gora(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o jedno pole w prawo od robota
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz1Prawo(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o dwa pola w prawo od robota
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz2Prawo(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o trzy pola w prawo od robota
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz3Prawo(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o jedno pole w lewo od robota
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz1Lewo(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o dwa pola w lewo od robota
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz2Lewo(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o trzy pola w lewo od robota
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz3Lewo(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o jedno pole pod robotem
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz1Dol(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o dwa pole pod robotem
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz2Dol(int miejsceRobota, int rozmiarBoku);

/*
 *Wskazuje miejsce na mapie o trzy pole pod robotem
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@return indeks wskazanego pola
 *
*/
int zaznacz3Dol(int miejsceRobota, int rozmiarBoku);

/*
 *Czysci dwa pola powyżej w lini prostej od robota jeśli na którymś nie odczytano obiektu
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *
*/
void czyscGora(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);

/*
 *Czysci dwa pola po prawej w lini prostej od robota jeśli na którymś nie odczytano obiektu
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *
*/
void czyscPrawo(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);

/*
 *Czysci dwa pola po lewej w lini prostej od robota jeśli na którymś nie odczytano obiektu
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *
*/
void czyscLewo(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);

/*
 *Czysci dwa pola poniżej w lini prostej od robota jeśli na którymś nie odczytano obiektu
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *
*/
void czyscDol(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku);


//Funkcje samodzielnej jazdy robota----------------------------------------------------------------------------

/*
 *Ustalanie sasiadow, czyli opisanie kazdego wezla jego otoczeniem na podstawie danej mapy
 *@param tablica[] aktualna mapa otoczenia
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *
*/
void ustalSasiadow(Wezel tablica[], int rozmiarTablicy, int rozmiarBoku);

/*
 *Wyznaczanie trasy
 *@param pPunktWejscia punkt z którego następuje próba wyznaczenia trasy
 *@param pPunktWyjscia punkt docelowy wyznaczania trasy
 *@param tablica[] aktualna mapa otoczenia
 *@return informacja o powodzeniu wyznaczania trasy
 *
*/
bool wyznaczTrase(int pPunktWejscia, int pPunktWyjscia, Wezel tablica[]);

/*
 *Wykonanie jednego ruchu do celu zgodnie z wyznaczona trasa. Ruch o jedno pole w pewnym kierunku.
 *@param tablica[] aktualna mapa otoczenia
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param pozycjaRobota kierunek, w którym ustawiony jest robot względem krawędzi tablicy mapy
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@param servo1 serwomechanizm pierwszego koła
 *@param servo2 serwomechanizm drugiego koła
 *@return obiekt określający aktualne miejsce i pozycję robota
*/
Robot wykonajRuchDoCelu(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2);

/*
 *Wybranie zbioru A celów do odkrycia. Są to 4 cele rozmieszczone po jednym na każdy róg mapy
 *@param tablicaCelow[] tablica zawierająca aktualne cele do zdobycia przez robota
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
*/
void wybierzCeleA(int tablicaCelow[], int rozmiarTablicy, int rozmiarBoku);


/*
 *Wybranie zbioru B celów do odkrycia. Są to cele rozmieszczone na pionowych krawędziach mapy od góry w kolejności spiralnej
 *@param tablicaCelow[] tablica zawierająca aktualne cele do zdobycia przez robota
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
*/
void wybierzCeleB(int tablicaCelow[], int rozmiarTablicy, int rozmiarBoku);

/*
 *Wybranie zbioru C celów do odkrycia. Są to cele rozmieszczone na każdym wolnym polu mapy od góry w kolejności spiralnej
 *@param tablicaCelow[] tablica zawierająca aktualne cele do zdobycia przez robota
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
*/
void wybierzCeleC(int tablicaCelow[], int rozmiarTablicy, int rozmiarBoku);

/*
 *Wyznaczenie nowego celu na mapie dla robota. Po wybraniu celu z początku, usunięcie go i przerzucenie pozostalych o 1 do pocczatku
 *@param tablica[] aktualna mapa otoczenia
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
 *@param tablicaCelow[] tablica zawierająca aktualne cele do zdobycia przez robota
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param pozycjaRobota kierunek, w którym ustawiony jest robot względem krawędzi tablicy mapy
 *@return miejsce wyznaczenia celu
*/
int wyznaczCel(Wezel tablica[], int rozmiarBoku, int tablicaCelow[], int rozmiarTablicy, char pozycjaRobota);

/*
 *Przesuniecie wybranego celu na ostanie miejsce tablicy celow, czy za ostatnim celem
 *@param cel miejsce określone jako cel
 *@param tablicaCelow[] tablica zawierająca aktualne cele do zdobycia przez robota
 *@param rozmiarTablicy całkowita liczba pól tablicy
*/
void przesunCelNaKoniec(int cel, int tablicaCelow[], int rozmiarTablicy);

/*
 *Tablica celów dostaje jeden cel jakim jest srodek mapy
 *@param miejsceRobota indeks pola na którym znajduje się robot
 *@param tablicaCelow[] tablica zawierająca aktualne cele do zdobycia przez robota
 *@param rozmiarTablicy całkowita liczba pól tablicy
 *@param rozmiarBoku liczba pól skądających się na jedną krawędź
*/
void wrocNaPoczatek(int miejsceStartRobota, int tablicaCelow[], int rozmiarTablicy, int rozmiarBoku);

//Funkcje pomocnicze-------------------------------------------------------------------------------

//Sprawdzenie czy podany element wystpil w tablicy
bool wystapilWTablicy(int tablica[], int element, int rozmiarTablicy);
//Sprawdzenie czy podany węzeł jest ścianą
bool czyWezelJestSciana(Wezel tablica[], int badanyWezel, int rozmiarTablicy, int rozmiarBoku);
//Przywraca wszystkie węzły do wartości inicjalnej
void czyscTablice(int tablica[], int rozmiarTablicy);
//Dekoduje podany znak na współrzędną numeryczną według ustalonego schematu
int wyznaczWspolrzedna(char znakWsp);
//Oblicza indeks pola na podstawie danej współrzędnej X, Y.
int obliczWspolrzedne(int wspolrzednaX, int wspolrzednaY, int rozmiarBoku);
//Sprawdza czy dane pole jest wolne
bool sparwdzDostepnoscMiejsca(Wezel tablica[], int miejsceObiektu, char pozycjaRobota);
//Usuwa z mapy wszystkie znaki oznaczające trasę
void czyscTrase(Wezel tablica[], int rozmiarTablicy);


//Pisanie na port szeregowy
void wyslijStringJson(Wezel tablica[], int rozmiarTablicy, int zestawDanych);

#endif
