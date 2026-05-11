#ifndef WCZYTYWANIEKONFIGURACJI_H
#define WCZYTYWANIEKONFIGURACJI_H

#include <string>

using namespace std;

class WczytywanieKonfiguracji {
public:
    // Zmienne ogolne
    bool wyswietlaj_pasek;
    string plik_wynikowy;
    int powtorzenia;

    // Zmienne Tabu Search
    int ts_max_iteracji;
    int ts_kadencja;
    bool ts_aspiracja_plus;
    bool ts_zmienna_kadencja;
    bool ts_uzyj_ub;
    bool ts_uzyj_lb;
    string ts_ub_metoda;

    // Pojedyncza sciezka do macierzy!
    string sciezka_do_pliku;

    WczytywanieKonfiguracji();
    bool wczytaj_z_pliku(const string& nazwa_pliku);
};

#endif // WCZYTYWANIEKONFIGURACJI_H