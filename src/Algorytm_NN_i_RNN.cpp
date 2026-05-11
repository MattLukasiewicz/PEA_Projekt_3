#include "Algorytm_NN_i_RNN.h"
#include <vector>
#include <limits>

using namespace std;

namespace {
    const int INF_KOSZT = numeric_limits<int>::max() / 4;

    bool czy_krawedz_ok(int koszt) {
        return koszt >= 0 && koszt < INF_KOSZT;
    }

    void rnn_rekurencja(const vector<vector<int>>& graf,
                        int n,
                        int start,
                        int obecny,
                        vector<bool>& odwiedzone,
                        vector<int>& aktualna_trasa,
                        int aktualny_koszt,
                        int& najlepszy_koszt,
                        vector<int>& najlepsza_trasa) {
        if (aktualny_koszt >= najlepszy_koszt) return;

        if (static_cast<int>(aktualna_trasa.size()) == n) {
            int koszt_powrotu = graf[obecny][start];
            if (!czy_krawedz_ok(koszt_powrotu)) return;
            int koszt_calkowity = aktualny_koszt + koszt_powrotu;
            if (koszt_calkowity < najlepszy_koszt) {
                najlepszy_koszt = koszt_calkowity;
                najlepsza_trasa = aktualna_trasa;
            }
            return;
        }

        int min_koszt = INF_KOSZT;
        for (int i = 0; i < n; ++i) {
            if (!odwiedzone[i]) {
                int koszt = graf[obecny][i];
                if (czy_krawedz_ok(koszt) && koszt < min_koszt) {
                    min_koszt = koszt;
                }
            }
        }

        if (min_koszt == INF_KOSZT) return;

        for (int i = 0; i < n; ++i) {
            if (!odwiedzone[i] && graf[obecny][i] == min_koszt) {
                odwiedzone[i] = true;
                aktualna_trasa.push_back(i);
                rnn_rekurencja(graf,
                               n,
                               start,
                               i,
                               odwiedzone,
                               aktualna_trasa,
                               aktualny_koszt + min_koszt,
                               najlepszy_koszt,
                               najlepsza_trasa);
                aktualna_trasa.pop_back();
                odwiedzone[i] = false;
            }
        }
    }

    RnnWynik rozwiaz_rnn(const vector<vector<int>>& graf) {
        int n = static_cast<int>(graf.size());
        if (n == 0) return {0, {}};

        int najlepszy_koszt = INF_KOSZT;
        vector<int> najlepsza_trasa;

        for (int start = 0; start < n; ++start) {
            vector<bool> odwiedzone(n, false);
            vector<int> aktualna_trasa;
            odwiedzone[start] = true;
            aktualna_trasa.push_back(start);

            rnn_rekurencja(graf,
                           n,
                           start,
                           start,
                           odwiedzone,
                           aktualna_trasa,
                           0,
                           najlepszy_koszt,
                           najlepsza_trasa);
        }

        if (najlepsza_trasa.empty()) {
            return {INF_KOSZT, {}};
        }

        return {najlepszy_koszt, najlepsza_trasa};
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
    RnnWynik wynik = rozwiaz_rnn(macierz.dane);
    if (wynik.koszt >= INF_KOSZT) return INF;
    return wynik.koszt;
}

int rozwiazAlgorytm_NN(const Macierz& macierz) {
    return algorytm_nn(macierz);
}

vector<int> oblicz_UB_RNN(const vector<vector<int>>& graf) {
    RnnWynik wynik = rozwiaz_rnn(graf);
    if (wynik.trasa.empty()) {
        return oblicz_UB_NN(graf);
    }
    return wynik.trasa;
}

vector<int> oblicz_UB_NN(const vector<vector<int>>& graf) {
    int n = static_cast<int>(graf.size());
    vector<int> trasa;
    if (n == 0) return trasa;

    vector<bool> odwiedzone(n, false);

    int obecny = 0;
    trasa.push_back(obecny);
    odwiedzone[obecny] = true;

    for (int i = 1; i < n; ++i) {
        int nastepny = -1;
        int min_dystans = numeric_limits<int>::max();

        for (int j = 0; j < n; ++j) {
            if (!odwiedzone[j] && graf[obecny][j] < min_dystans) {
                min_dystans = graf[obecny][j];
                nastepny = j;
            }
        }

        if (nastepny == -1) {
            return {};
        }

        obecny = nastepny;
        trasa.push_back(obecny);
        odwiedzone[obecny] = true;
    }

    return trasa;
}