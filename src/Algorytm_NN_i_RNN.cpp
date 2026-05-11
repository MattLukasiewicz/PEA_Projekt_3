#include "Algorytm_NN_i_RNN.h"
#include <vector>

using namespace std;

static void rnnRekurencja(const Macierz& macierz,
                         int n,
                         int start,
                         int obecny,
                         vector<bool>& odwiedzone,
                         int odwiedzonych,
                         int aktualnyKoszt,
                         int& najlepszyWynik) {
    if (aktualnyKoszt >= najlepszyWynik) return;

    if (odwiedzonych == n) {
        if (macierz.dane[obecny][start] == INF) return;

        int kosztKoncowy = aktualnyKoszt + macierz.dane[obecny][start];
        if (kosztKoncowy < najlepszyWynik) {
            najlepszyWynik = kosztKoncowy;
        }
        return;
    }

    int minKoszt = INF;
    for (int i = 0; i < n; i++) {
        if (odwiedzone[i] == false && macierz.dane[obecny][i] != INF) {
            if (macierz.dane[obecny][i] < minKoszt) {
                minKoszt = macierz.dane[obecny][i];
            }
        }
    }

    if (minKoszt == INF) return;

    for (int i = 0; i < n; i++) {
        if (odwiedzone[i] == false && macierz.dane[obecny][i] == minKoszt) {
            odwiedzone[i] = true;
            rnnRekurencja(macierz,
                          n,
                          start,
                          i,
                          odwiedzone,
                          odwiedzonych + 1,
                          aktualnyKoszt + minKoszt,
                          najlepszyWynik);
            odwiedzone[i] = false;
        }
    }
}

int algorytm_nn(const Macierz& macierz) {
    int n = macierz.rozmiar;
    if (n <= 1) return 0;

    vector<bool> odwiedzone(n, false);
    int obecny = 0;
    odwiedzone[obecny] = true;
    
    int kosztCalkowity = 0;
    int odwiedzonych = 1;

    while (odwiedzonych < n) {
        int minKoszt = INF; 
        int nastepny = -1;

        for (int i = 0; i < n; i++) {
            if (odwiedzone[i] == false && macierz.dane[obecny][i] != INF) {
                if (macierz.dane[obecny][i] < minKoszt) {
                    minKoszt = macierz.dane[obecny][i];
                    nastepny = i;
                }
            }
        }

        if (nastepny == -1) return INF;

        odwiedzone[nastepny] = true;
        kosztCalkowity += minKoszt;
        obecny = nastepny;
        odwiedzonych++;
    }

    if (macierz.dane[obecny][0] == INF) return INF;
    kosztCalkowity += macierz.dane[obecny][0]; 
    
    return kosztCalkowity;
}

int algorytm_rnn(const Macierz& macierz) {
    int n = macierz.rozmiar;
    if (n <= 1) return 0;

    int najlepszyWynik = INF;
    for (int start = 0; start < n; start++) {
        vector<bool> odwiedzone(n, false);
        odwiedzone[start] = true;
        rnnRekurencja(macierz, n, start, start, odwiedzone, 1, 0, najlepszyWynik);
    }

    return najlepszyWynik;
}

int rozwiazAlgorytm_NN(const Macierz& macierz) {
    return algorytm_nn(macierz);
}