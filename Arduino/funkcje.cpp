#include <Servo.h>
#include "funkcje.h"
#include "Arduino.h"



//---------------------------------------------------------------------------------------
//Implementacja
void jedzProsto(Servo servo1, Servo servo2) {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(45);
	servo2.write(135);
}
void skrecajWLewo(Servo servo1, Servo servo2) {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(45);
	servo2.write(45);
}
void skrecajWPrawo(Servo servo1, Servo servo2) {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(135);
	servo2.write(135);
}
void jedzDoTylu(Servo servo1, Servo servo2) {
	servo1.attach(5);
	servo2.attach(6);
	servo1.write(135);
	servo2.write(45);
}
void zatrzymajRuch(Servo servo1, Servo servo2) {
	servo1.detach();
	servo2.detach();
}
void zatrzymajGlowe(Servo servo3) {
	servo3.detach();
}
void glowaLewo(Servo servo3) {
	servo3.attach(3);
	servo3.write(45);
}
void glowaPrawo(Servo servo3) {
	servo3.attach(3);
	servo3.write(135);
}

int wykonajJedenRuchPrzod(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2) {

	//Tutaj kod serwomechanizmów	
	jedzProsto(servo1, servo2);
        delay(1170);
        zatrzymajRuch(servo1, servo2);
	
	//Odwzorowanie na mapie
	int wskazGdzieZaznaczyc = miejsceRobota;
	switch (pozycjaRobota) {
		case (PRZODEM_GORA): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - rozmiarBoku; break;
		case (PRZODEM_LEWO): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - 1; break;
		case (PRZODEM_PRAWO): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + 1; break;
		case (PRZODEM_DOL): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + rozmiarBoku; break;
	}
	char znakBadany = tablica[wskazGdzieZaznaczyc].rodzajWezla;
	if (wskazGdzieZaznaczyc != miejsceRobota && znakBadany != ZNAK_SCIANA && wskazGdzieZaznaczyc >= 0 && wskazGdzieZaznaczyc < rozmiarTablicy) {
		tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
		miejsceRobota = wskazGdzieZaznaczyc;
		tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
	}
	return miejsceRobota;
}
int wykonajJedenRuchTyl(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2) {

	jedzDoTylu(servo1, servo2);
        delay(1170);
        zatrzymajRuch(servo1, servo2);
	
	//Odwzorowanie na mapie
	int wskazGdzieZaznaczyc = miejsceRobota;
	switch (pozycjaRobota) {
		case (PRZODEM_GORA): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + rozmiarBoku; break;
		case (PRZODEM_LEWO): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc + 1; break;
		case (PRZODEM_PRAWO): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - 1; break;
		case (PRZODEM_DOL): wskazGdzieZaznaczyc = wskazGdzieZaznaczyc - rozmiarBoku; break;
	}
	char znakBadany = tablica[wskazGdzieZaznaczyc].rodzajWezla;
	if (wskazGdzieZaznaczyc != miejsceRobota && znakBadany != ZNAK_SCIANA && wskazGdzieZaznaczyc >= 0 && wskazGdzieZaznaczyc < rozmiarTablicy) {
		tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
		miejsceRobota = wskazGdzieZaznaczyc;
		tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
	}
	return miejsceRobota;
}
char wykonajObrot90Lewo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2) {


	skrecajWLewo(servo1, servo2);
        delay(500);
        zatrzymajRuch(servo1, servo2);

	//Odwzorowanie na mapie
	switch (pozycjaRobota) {
		case (PRZODEM_GORA): pozycjaRobota = PRZODEM_LEWO; break;
		case (PRZODEM_LEWO): pozycjaRobota = PRZODEM_DOL; break;
		case (PRZODEM_PRAWO): pozycjaRobota = PRZODEM_GORA; break;
		case (PRZODEM_DOL): pozycjaRobota = PRZODEM_PRAWO; break;
	}
	tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
	return pozycjaRobota;
}
char wykonajObrot90Prawo(Wezel tablica[], int miejsceRobota, char pozycjaRobota, Servo servo1, Servo servo2) {


	skrecajWPrawo(servo1, servo2);
        delay(500);
        zatrzymajRuch(servo1, servo2);
	//

	//Odwzorowanie na mapie
	switch (pozycjaRobota) {
		case (PRZODEM_GORA): pozycjaRobota = PRZODEM_PRAWO; break;
		case (PRZODEM_LEWO): pozycjaRobota = PRZODEM_GORA; break;
		case (PRZODEM_PRAWO): pozycjaRobota = PRZODEM_DOL; break;
		case (PRZODEM_DOL): pozycjaRobota = PRZODEM_LEWO; break;
	}
	tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
	return pozycjaRobota;
}

double skanujSensorPrzedni(int IRFront) {
	double volts = analogRead(IRFront)*0.004692082;   
	double odlegloscZmierzona =(14.925-(1.5*volts))/(volts+0.05);  
	return odlegloscZmierzona;
}
double skanujSensorLewy(int IRLeft) {
	double volts = analogRead(IRLeft)*0.004692082;   
	double odlegloscZmierzona =(14.925-(1.5*volts))/(volts+0.05);  
	return odlegloscZmierzona;
}
double skanujSensorPrawy(int IRRight) {
	double volts = analogRead(IRRight)*0.004692082;   
	double odlegloscZmierzona =(14.925-(1.5*volts))/(volts+0.05);  
	return odlegloscZmierzona;
}

int sprawdzOdlegloscIZaznacz(double odleglosc, int miejsceRobota, char pozycjaCzujnika, Wezel tablica[], int rozmiarTablicy, int rozmiarBoku) {

	if (odleglosc > IR_MIN && odleglosc <= 20.0) {
		switch (pozycjaCzujnika) {
			case (PRZODEM_GORA): return zaznacz1Gora(miejsceRobota, rozmiarBoku);
			case (PRZODEM_PRAWO): return zaznacz1Prawo(miejsceRobota, rozmiarBoku);
			case (PRZODEM_LEWO): return zaznacz1Lewo(miejsceRobota, rozmiarBoku);
			case (PRZODEM_DOL): return zaznacz1Dol(miejsceRobota, rozmiarBoku);
		}
	} else if (odleglosc > 20.0 && odleglosc <= IR_MAX) {
		switch (pozycjaCzujnika) {
			case (PRZODEM_GORA): return zaznacz2Gora(miejsceRobota, rozmiarBoku);
			case (PRZODEM_PRAWO): return zaznacz2Prawo(miejsceRobota, rozmiarBoku);
			case (PRZODEM_LEWO): return zaznacz2Lewo(miejsceRobota, rozmiarBoku);
			case (PRZODEM_DOL): return zaznacz2Dol(miejsceRobota, rozmiarBoku);
		}
/*		
	} else if (odleglosc > 20.0 && odleglosc <= 30.0) {
		switch (pozycjaCzujnika) {
			case (PRZODEM_GORA): return zaznacz3Gora(miejsceRobota, rozmiarBoku);
			case (PRZODEM_PRAWO): return zaznacz3Prawo(miejsceRobota, rozmiarBoku);
			case (PRZODEM_LEWO): return zaznacz3Lewo(miejsceRobota, rozmiarBoku);
			case (PRZODEM_DOL): return zaznacz3Dol(miejsceRobota, rozmiarBoku);
		}
*/		
	} else {
		switch (pozycjaCzujnika) {
			case (PRZODEM_GORA): czyscGora(tablica, miejsceRobota, rozmiarTablicy, rozmiarBoku); break;
			case (PRZODEM_PRAWO): czyscPrawo(tablica, miejsceRobota, rozmiarTablicy, rozmiarBoku); break;
			case (PRZODEM_LEWO): czyscLewo(tablica, miejsceRobota, rozmiarTablicy, rozmiarBoku); break;
			case (PRZODEM_DOL): czyscDol(tablica, miejsceRobota, rozmiarTablicy, rozmiarBoku); break;
		}
		return miejsceRobota;
	}
}

void inicjujMape(int rozmiarMapy, int rozmiarBoku, int miejsceRobota, char pozycjaRobota, Wezel tablica[]) {
	for (int i=0; i<rozmiarBoku; i++) {
		tablica[i].rodzajWezla = ZNAK_SCIANA;
	}
	for (int i=rozmiarBoku; i<rozmiarMapy-rozmiarBoku; i++) {
		if (i%rozmiarBoku == 0 || i%rozmiarBoku == rozmiarBoku - 1) {
			tablica[i].rodzajWezla = ZNAK_SCIANA;
		} else {
			tablica[i].rodzajWezla = ZNAK_WOLNE;
		}
	}
	for (int i=rozmiarMapy-rozmiarBoku; i<rozmiarMapy; i++) {
		tablica[i].rodzajWezla = ZNAK_SCIANA;
	}
	miejsceRobota = (rozmiarMapy + rozmiarBoku) /2;
	tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
}

bool skanujZaznaczMape(int miejsceRobota, char pozycjaCzujnikaPrzod, Wezel tablica[], int rozmiarTablicy, int rozmiarBoku) {

	char pozycjaCzujnikaLewy;
	char pozycjaCzujnikaPrawy;
	bool wykrytoElement = false;
	
	double s1 = 35.0;
	double s2 = 1.0;
	double s3 = 25.0;
	
	//Ustalenie pozycji w jakiej s czujniki
	switch (pozycjaCzujnikaPrzod) {
		case (PRZODEM_GORA):
			pozycjaCzujnikaLewy = PRZODEM_LEWO;
			pozycjaCzujnikaPrawy = PRZODEM_PRAWO;
			break;
		case (PRZODEM_LEWO):
			pozycjaCzujnikaLewy = PRZODEM_DOL;
			pozycjaCzujnikaPrawy = PRZODEM_GORA;
			break;
		case (PRZODEM_PRAWO):
			pozycjaCzujnikaLewy = PRZODEM_GORA;
			pozycjaCzujnikaPrawy = PRZODEM_DOL;
			break;
		case (PRZODEM_DOL):
			pozycjaCzujnikaLewy = PRZODEM_PRAWO;
			pozycjaCzujnikaPrawy = PRZODEM_LEWO;
			break;
	}
	
	int wskazGdzieZaznaczycPrzod = sprawdzOdlegloscIZaznacz(s1, miejsceRobota, pozycjaCzujnikaPrzod, tablica, rozmiarTablicy, rozmiarBoku);
	int wskazGdzieZaznaczycLewo = sprawdzOdlegloscIZaznacz(s2, miejsceRobota, pozycjaCzujnikaLewy, tablica, rozmiarTablicy, rozmiarBoku);
	int wskazGdzieZaznaczycPrawo = sprawdzOdlegloscIZaznacz(s3, miejsceRobota, pozycjaCzujnikaPrawy, tablica, rozmiarTablicy, rozmiarBoku);

	char znakBadany = tablica[wskazGdzieZaznaczycPrzod].rodzajWezla;
	if (wskazGdzieZaznaczycPrzod != miejsceRobota && tablica[wskazGdzieZaznaczycPrzod].rodzajWezla != ZNAK_SCIANA && wskazGdzieZaznaczycPrzod >= 0 && wskazGdzieZaznaczycPrzod < rozmiarTablicy) {
		tablica[wskazGdzieZaznaczycPrzod].rodzajWezla = ZNAK_SCIANA;
		wykrytoElement = true;
	}
	if (wskazGdzieZaznaczycLewo != miejsceRobota && tablica[wskazGdzieZaznaczycLewo].rodzajWezla != ZNAK_SCIANA && wskazGdzieZaznaczycLewo >= 0 && wskazGdzieZaznaczycLewo < rozmiarTablicy) {
		tablica[wskazGdzieZaznaczycLewo].rodzajWezla = ZNAK_SCIANA;
		wykrytoElement = true;
	}
	if (wskazGdzieZaznaczycPrawo != miejsceRobota && tablica[wskazGdzieZaznaczycPrawo].rodzajWezla != ZNAK_SCIANA && wskazGdzieZaznaczycPrawo >= 0 && wskazGdzieZaznaczycPrawo < rozmiarTablicy) {
		tablica[wskazGdzieZaznaczycPrawo].rodzajWezla = ZNAK_SCIANA;
		wykrytoElement = true;
	}
	return wykrytoElement;
}

int zaznacz1Gora(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota - rozmiarBoku;
	return miejsceZaznaczenia;
}
int zaznacz2Gora(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota - (2 * rozmiarBoku);
	return miejsceZaznaczenia;
}
int zaznacz3Gora(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota - (3 * rozmiarBoku);
	return miejsceZaznaczenia;
}
int zaznacz1Lewo(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota - 1;
	return miejsceZaznaczenia;
}
int zaznacz2Lewo(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota - 2;
	return miejsceZaznaczenia;
}
int zaznacz3Lewo(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota - 3;
	return miejsceZaznaczenia;
}
int zaznacz1Prawo(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota + 1;
	return miejsceZaznaczenia;
}
int zaznacz2Prawo(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota + 2;
	return miejsceZaznaczenia;
}
int zaznacz3Prawo(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota + 3;
	return miejsceZaznaczenia;
}
int zaznacz1Dol(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota + rozmiarBoku;
	return miejsceZaznaczenia;
}
int zaznacz2Dol(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota + (2 * rozmiarBoku);
	return miejsceZaznaczenia;
}
int zaznacz3Dol(int miejsceRobota, int rozmiarBoku) {
	int miejsceZaznaczenia = miejsceRobota + (3 * rozmiarBoku);
	return miejsceZaznaczenia;
}
//Czysci dwa pola w lini prostej od robota (jesli czujnik obejmie wicej niz 2 kratki trzeba duzych zmian)
void czyscGora(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku) {
	int badanyWezel = miejsceRobota - rozmiarBoku;
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
	badanyWezel = miejsceRobota - (2*rozmiarBoku);
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
}
void czyscPrawo(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku) {
	int badanyWezel = miejsceRobota + 1;
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
	badanyWezel = miejsceRobota + 2;
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
}
void czyscLewo(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku) {
	int badanyWezel = miejsceRobota - 1;
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
	badanyWezel = miejsceRobota - 2;
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
}
void czyscDol(Wezel tablica[], int miejsceRobota, int rozmiarTablicy, int rozmiarBoku) {
	int badanyWezel = miejsceRobota + rozmiarBoku;
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
	badanyWezel = miejsceRobota + (2*rozmiarBoku);
	if (czyWezelJestSciana(tablica, badanyWezel, rozmiarTablicy, rozmiarBoku)) {
		tablica[badanyWezel].rodzajWezla = ZNAK_WOLNE;
	}
}

//Ustalanie sasiadow, czyli opisanie kazdego wezla jego otoczeniem, po wygenerowaniu labiryntu
void ustalSasiadow(Wezel tablica[], int rozmiarTablicy, int rozmiarBoku) {
	//inicjalne ustawienie braku sasiadow i poprzednika dla wszystkich wezlow
	for (int i=0; i<rozmiarTablicy; i++) {
		tablica[i].numerPoprzednika = BRAK_WEZLA;
		tablica[i].sasiadZLewej = BRAK_WEZLA;
		tablica[i].sasiadZPrawej = BRAK_WEZLA;
		tablica[i].sasiadZGory = BRAK_WEZLA;
		tablica[i].sasiadZDolu = BRAK_WEZLA;

		if (tablica[i].rodzajWezla == ZNAK_TRASA) {
			tablica[i].rodzajWezla = ZNAK_WOLNE;
		}
	}
	//ustalenie sasiadow dla wszystkich wezlow na podstawie rozmieszczenia wezlow typu SCIANA
	for (int i=0; i<rozmiarTablicy; i++) { 
		if (i == 0) {//wariant 1: punkt (0,0)
			if (tablica[i + 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZPrawej = i + 1;
			if (tablica[i + rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZDolu = i + rozmiarBoku;
		}
		else if (i == PG_ROG) {//wariant 2: prawy gy rg
			if (tablica[i - 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZLewej = i - 1;
			if (tablica[i + rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZDolu = i + rozmiarBoku;
		}
		else if (i == LD_ROG) {//wariant 3: lewy dolny rg
			if (tablica[i + 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZPrawej = i + 1;
			if (tablica[i - rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZGory = i - rozmiarBoku;
		}
		else if (i == PD_ROG) {//wariant 4: prawy dolny rg
			if (tablica[i - 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZLewej = i - 1;
			if (tablica[i - rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZGory = i - rozmiarBoku;
		}
		else if (i > 0 && i < PG_ROG) {//wariant 5: grny bok
			if (tablica[i - 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZLewej = i - 1;
			if (tablica[i + 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZPrawej = i + 1;
			if (tablica[i + rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZDolu = i + rozmiarBoku;
		}
		else if (i > LD_ROG && i < PD_ROG) {//wariant: dolny bok
			if (tablica[i - 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZLewej = i - 1;
			if (tablica[i + 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZPrawej = i + 1;
			if (tablica[i - rozmiarBoku].rodzajWezla != ZNAK_SCIANA )
				tablica[i].sasiadZGory = i - rozmiarBoku;
		}
		else if (i % rozmiarBoku == 0) {// wariant 7: lewy bok
			if (tablica[i + 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZPrawej = i + 1;
			if (tablica[i + rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZDolu = i + rozmiarBoku;
			if (tablica[i - rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZGory = i - rozmiarBoku;
		}
		else if (i % rozmiarBoku == rozmiarBoku - 1) {//wariant 8: prawy bok
			if (tablica[i - 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZLewej = i - 1;
			if (tablica[i + rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZGory = i + rozmiarBoku;
			if (tablica[i - rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZDolu = i - rozmiarBoku;
		}
		else { //wariant 9: rodek
			if (tablica[i - 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZLewej = i - 1;
			if (tablica[i + 1].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZPrawej = i + 1;
			if (tablica[i + rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZDolu = i + rozmiarBoku;
			if (tablica[i - rozmiarBoku].rodzajWezla != ZNAK_SCIANA)
				tablica[i].sasiadZGory = i - rozmiarBoku;
		}
	}
}

//wyznaczanie trasy
void wyznaczTrase(int pPunktWejscia, int pPunktWyjscia, Wezel tablica[]) {
	int tablDoSprawdzenia[ROZMIAR_MAPY];

	int punktSprawdzania = 0;
	int punktDodanyDoSpr = 1;
	int sasiad = BRAK_WEZLA;

	tablDoSprawdzenia[0] = pPunktWejscia;
	
	while (punktSprawdzania != punktDodanyDoSpr && tablDoSprawdzenia[punktSprawdzania] != pPunktWyjscia) {
		for (int i = 1; i <= 4; i++) { // dla kadego ewentualnego sasiada
			switch (i) {
				case 1:	sasiad = tablica[ tablDoSprawdzenia[punktSprawdzania] ].sasiadZLewej; break;
				case 2:	sasiad = tablica[ tablDoSprawdzenia[punktSprawdzania] ].sasiadZPrawej; break;
				case 3:	sasiad = tablica[ tablDoSprawdzenia[punktSprawdzania] ].sasiadZGory; break;
				case 4:	sasiad = tablica[ tablDoSprawdzenia[punktSprawdzania] ].sasiadZDolu; break;
			}
			
			if (sasiad != BRAK_WEZLA) { // sasiad istnieje
				//dodanie sasiada do tablicy do sprawdzenia, o ile dotychczas w niej nie wystpowal
				if (!wystapilWTablicy(tablDoSprawdzenia, sasiad, punktDodanyDoSpr)) {
					tablDoSprawdzenia[punktDodanyDoSpr] = sasiad;
					punktDodanyDoSpr = punktDodanyDoSpr + 1;
					tablica[sasiad].numerPoprzednika = tablDoSprawdzenia[punktSprawdzania];
				}
			}
		}

		punktSprawdzania = punktSprawdzania + 1;
	}

	punktSprawdzania = pPunktWyjscia;
	while (punktSprawdzania != pPunktWejscia && tablica[punktSprawdzania].numerPoprzednika != BRAK_WEZLA) {
		punktSprawdzania = tablica[punktSprawdzania].numerPoprzednika;
		if (punktSprawdzania != pPunktWejscia) {
			tablica[punktSprawdzania].rodzajWezla = ZNAK_TRASA;
		}
	}
}

//Sprawdzenie czy podany element wystpil w tablicy
bool wystapilWTablicy(int tablica[], int element, int rozmiarTablicy) {
	bool jestWTablicy = false;
	int i = 0;
	do {
		if (tablica[i] == element)
			jestWTablicy = true;
		i++;
	} while (jestWTablicy == false && i < rozmiarTablicy);
	
	return jestWTablicy;
}

//Wykonanie jednego ruchu do celu zgodnie z wyznaczona trasa
Robot wykonajRuchDoCelu(Wezel tablica[], int miejsceRobota, char pozycjaRobota, int rozmiarTablicy, int rozmiarBoku, Servo servo1, Servo servo2) {
	Robot robot;
	if (tablica[tablica[miejsceRobota].sasiadZDolu].rodzajWezla == ZNAK_TRASA || tablica[tablica[miejsceRobota].sasiadZDolu].rodzajWezla == ZNAK_CEL) {
		switch (pozycjaRobota) {
			case (PRZODEM_DOL):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_GORA):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_LEWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Lewo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_PRAWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
		}

	} else if (tablica[tablica[miejsceRobota].sasiadZGory].rodzajWezla == ZNAK_TRASA || tablica[tablica[miejsceRobota].sasiadZGory].rodzajWezla == ZNAK_CEL) {
		switch (pozycjaRobota) {
			case (PRZODEM_DOL):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_GORA):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_LEWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_PRAWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Lewo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
		}
	} else if (tablica[tablica[miejsceRobota].sasiadZLewej].rodzajWezla == ZNAK_TRASA || tablica[tablica[miejsceRobota].sasiadZLewej].rodzajWezla == ZNAK_CEL) {
		switch (pozycjaRobota) {
			case (PRZODEM_DOL):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_GORA):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Lewo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_LEWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_PRAWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
		}
	} else if (tablica[tablica[miejsceRobota].sasiadZPrawej].rodzajWezla == ZNAK_TRASA || tablica[tablica[miejsceRobota].sasiadZPrawej].rodzajWezla == ZNAK_CEL) {
		switch (pozycjaRobota) {
			case (PRZODEM_DOL):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Lewo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_GORA):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_LEWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				pozycjaRobota = wykonajObrot90Prawo(tablica, miejsceRobota, pozycjaRobota, servo1, servo2);
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
			case (PRZODEM_PRAWO):
				tablica[miejsceRobota].rodzajWezla = ZNAK_WOLNE;
				miejsceRobota = wykonajJedenRuchPrzod(tablica, miejsceRobota, pozycjaRobota, rozmiarTablicy, rozmiarBoku, servo1, servo2);
				tablica[miejsceRobota].rodzajWezla = pozycjaRobota;
				break;
		}
	}

	robot.miejsceRobota = miejsceRobota;
	robot.pozycjaRobota = pozycjaRobota;
	return robot;
}

//Wyznaczenie nowego celu na mapie dla robota
int wyznaczCel(Wezel tablica[]) {
	int cel = 11;
	tablica[cel].rodzajWezla = ZNAK_CEL;
	return cel;
}

bool czyWezelJestSciana(Wezel tablica[], int badanyWezel, int rozmiarTablicy, int rozmiarBoku) {
	if (tablica[badanyWezel].rodzajWezla == ZNAK_SCIANA && badanyWezel > rozmiarBoku && badanyWezel < rozmiarTablicy - rozmiarBoku && badanyWezel%rozmiarBoku != 0 && badanyWezel%rozmiarBoku != rozmiarBoku - 1) {
		return true;
	} else {
		return false;
	}
}


/*
void wyslijTablice(char tablica[], int rozmiarTablicy) {
	for (int i=0; i<rozmiarTablicy; i++) {
		Serial.print(tablica[i]);
	}
	Serial.println();
}
*/
void wyslijStringJson(Wezel tablica[], int rozmiarTablicy) {
	char tablicaChar[ROZMIAR_MAPY];
	for (int i = 0; i < rozmiarTablicy; i++) {
		tablicaChar[i] = tablica[i].rodzajWezla;
	}

	Serial.println("{ \"mapa\":\"" +String(tablicaChar)+ "\" }");
}

void beep(unsigned char delayms){
	analogWrite(11, 20);     
	delay(delayms);         
	analogWrite(11, 0);   
	delay(delayms);         
} 
