#include "WczytywanieKonfiguracji.h"
#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

WczytywanieKonfiguracji::WczytywanieKonfiguracji() {
    wyswietlaj_pasek = false;
    plik_wynikowy = "";
    powtorzenia = 1;
    
    ts_max_iteracji = 0;
    ts_kadencja = 0;
    ts_aspiracja_klasyczna = true;
    ts_aspiracja_plus = false;
    ts_zmienna_kadencja = false;
    ts_uzyj_ub = false;
    ts_uzyj_lb = false;
    ts_ub_metoda = "NN";
    
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
        if (klucz.substr(0, 2) == "//") {
            string reszta_linii;
            getline(plik, reszta_linii);
            continue;
        }

        plik >> znak_rownosci;

        if (klucz == "WYSWIETLAJ_PASEK_POSTEPU") {
            plik >> wyswietlaj_pasek;
        } else if (klucz == "PLIK_WYNIKOWY") {
            plik >> plik_wynikowy;
        }else if (klucz == "PLIK_KONWERGENCJI") {
            plik >> plik_konwergencji;
        } else if (klucz == "POWTORZENIA") {
            plik >> powtorzenia;
        } else if (klucz == "TS_MAX_ITERACJI") {
            plik >> ts_max_iteracji;
        } else if (klucz == "TS_KADENCJA") {
            plik >> ts_kadencja;
        } else if (klucz == "TS_ASPIRACJA_KLASYCZNA") {
            plik >> ts_aspiracja_klasyczna;
        } else if (klucz == "TS_ASPIRACJA_PLUS") {
            plik >> ts_aspiracja_plus;
        } else if (klucz == "TS_ZMIENNA_KADENCJA") {
            plik >> ts_zmienna_kadencja;
        } else if (klucz == "TS_UZYJ_UB") {
            plik >> ts_uzyj_ub;
        } else if (klucz == "TS_UZYJ_LB") {
            plik >> ts_uzyj_lb;
        } else if (klucz == "TS_UB_METODA") {
            plik >> ts_ub_metoda;
            for (char& c : ts_ub_metoda) {
                c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
            }
        } else if (klucz == "SCIEZKA_DO_PLIKU") {
            plik >> sciezka_do_pliku;
        }
    }

    plik.close();
    return true;
}