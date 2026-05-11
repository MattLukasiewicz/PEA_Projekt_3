#include "Macierze.h"

using namespace std;

Macierz::Macierz() : rozmiar(0), optymalnyKoszt(-1) {}
Macierz::Macierz(int n) : rozmiar(n), dane(n, vector<int>(n, 0)), optymalnyKoszt(-1) {}

void Macierz::wypisz() const {
    for (int i = 0; i < rozmiar; ++i) {
        for (int j = 0; j < rozmiar; ++j) {
            if (dane[i][j] == INF) cout << "INF\t";
            else cout << dane[i][j] << "\t";
        }
        cout << "\n";
    }
}

int Macierz::redukuj() {
    int kosztRedukcji = 0;

    for (int i = 0; i < rozmiar; ++i) {
        int minWiersza = INF;
        for (int j = 0; j < rozmiar; ++j) {
            if (dane[i][j] < minWiersza) {
                minWiersza = dane[i][j];
            }
        }
        if (minWiersza != INF && minWiersza > 0) {
            kosztRedukcji += minWiersza;
            for (int j = 0; j < rozmiar; ++j) {
                if (dane[i][j] != INF) {
                    dane[i][j] -= minWiersza;
                }
            }
        }
    }

    for (int j = 0; j < rozmiar; ++j) {
        int minKolumny = INF;
        for (int i = 0; i < rozmiar; ++i) {
            if (dane[i][j] < minKolumny) {
                minKolumny = dane[i][j];
            }
        }
        if (minKolumny != INF && minKolumny > 0) {
            kosztRedukcji += minKolumny;
            for (int i = 0; i < rozmiar; ++i) {
                if (dane[i][j] != INF) {
                    dane[i][j] -= minKolumny;
                }
            }
        }
    }
    return kosztRedukcji;
}