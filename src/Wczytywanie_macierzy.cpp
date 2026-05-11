#include "Wczytywanie_macierzy.h"
#include <fstream>
#include <iostream>

using namespace std;

Macierz Wczytywanie_Macierzy::wczytajMacierz(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cerr << "Wystapil blad: wczytywanie macierzy: " << nazwaPliku << "\n";
        return Macierz(0);
    }

    int n;
    plik >> n;

    Macierz mat(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int wartosc;
            plik >> wartosc;
            if (i == j || wartosc < 0) {
                mat.dane[i][j] = INF;
            } else {
                mat.dane[i][j] = wartosc;
            }
        }
    }

    string temp;
    while (plik >> temp) {
        size_t poz = temp.find("sum_min=");
        if (poz != string::npos) {
            mat.optymalnyKoszt = stoi(temp.substr(poz + 8));
            break;
        }
    }

    return mat;
}