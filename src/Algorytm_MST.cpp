#include "Algorytm_MST.h"
#include <limits>

using namespace std;

int oblicz_LB_MST(const vector<vector<int>>& graf) {
    if (graf.empty()) return 0;
    
    int rozmiar = graf.size();
    int koszt_calkowity = 0;
    const int MAX_WAGA = numeric_limits<int>::max();
    
    vector<bool> zbadane_wierzcholki(rozmiar, false);
    vector<int> najtansze_krawedzie(rozmiar, MAX_WAGA);
    
    najtansze_krawedzie[0] = 0;

    int dolaczone_wierzcholki = 0;
    while (dolaczone_wierzcholki < rozmiar) {
        int obecny_wezel = -1;
        int minimum_lokalne = MAX_WAGA;
        
        for (int i = 0; i < rozmiar; ++i) {
            if (!zbadane_wierzcholki[i] && najtansze_krawedzie[i] < minimum_lokalne) {
                minimum_lokalne = najtansze_krawedzie[i];
                obecny_wezel = i;
            }
        }

        if (obecny_wezel == -1) {
            break; 
        }

        zbadane_wierzcholki[obecny_wezel] = true;
        koszt_calkowity += najtansze_krawedzie[obecny_wezel];
        dolaczone_wierzcholki++;

        for (int sasiad = 0; sasiad < rozmiar; ++sasiad) {
            if (!zbadane_wierzcholki[sasiad]) {
                int waga_przod = graf[obecny_wezel][sasiad];
                int waga_tyl = graf[sasiad][obecny_wezel];
                
                int optymalna_krawedz = waga_przod;
                
                if (waga_tyl != -1) {
                    if (waga_przod == -1 || waga_tyl < waga_przod) {
                        optymalna_krawedz = waga_tyl;
                    }
                }

                if (optymalna_krawedz != -1 && optymalna_krawedz < najtansze_krawedzie[sasiad]) {
                    najtansze_krawedzie[sasiad] = optymalna_krawedz;
                }
            }
        }
    }

    return koszt_calkowity;
}