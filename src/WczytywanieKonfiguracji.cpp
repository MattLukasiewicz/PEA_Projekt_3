#include "WczytywanieKonfiguracji.h"
#include <fstream>
#include <iostream>

using namespace std;

// Konstruktor - samo zerowanie zmiennych
WczytywanieKonfiguracji::WczytywanieKonfiguracji() {
    wyswietlaj_pasek = false;
    plik_wynikowy = "";
    ilosc_instancji = 0;
    
    ts_max_iteracji = 0;
    ts_kadencja = 0;
    ts_aspiracja_plus = false;
    ts_zmienna_kadencja = false;
    
    sciezka_do_pliku = "";
}

bool WczytywanieKonfiguracji::wczytaj_z_pliku(const string& nazwa_pliku) {
    ifstream plik(nazwa_pliku);

    if (!plik.is_open()) {
        cerr << "Blad: Nie udalo sie otworzyc pliku configu: " << nazwa_pliku << endl;
        return false;
    }

    string klucz;
    string znak_rownosci;

    while (plik >> klucz) {
        // Ignorowanie komentarzy
        if (klucz.substr(0, 2) == "//") {
            string reszta_linii;
            getline(plik, reszta_linii);
            continue;
        }

        // Pochlanianie znaku równe się
        plik >> znak_rownosci;

        // Parsowanie parametrów
        if (klucz == "WYSWIETLAJ_PASEK_POSTEPU") {
            plik >> wyswietlaj_pasek;
        } else if (klucz == "PLIK_WYNIKOWY") {
            plik >> plik_wynikowy;
        } else if (klucz == "ILOSC_BADANYCH_INSTANCJI") {
            plik >> ilosc_instancji;
        } else if (klucz == "TS_MAX_ITERACJI") {
            plik >> ts_max_iteracji;
        } else if (klucz == "TS_KADENCJA") {
            plik >> ts_kadencja;
        } else if (klucz == "TS_ASPIRACJA_PLUS") {
            plik >> ts_aspiracja_plus;
        } else if (klucz == "TS_ZMIENNA_KADENCJA") {
            plik >> ts_zmienna_kadencja;
        } else if (klucz == "SCIEZKA_DO_PLIKU") {
            plik >> sciezka_do_pliku;
        }
    }

    plik.close();
    return true;
}