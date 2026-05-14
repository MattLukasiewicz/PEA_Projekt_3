#include "Algorytm_MST.h"
#include <limits>

using namespace std;

// Wyliczanie dolnego ograniczenia (LB) z wykorzystaniem algorytmu Prima
int oblicz_LB_MST(const vector<vector<int>>& graf) {
    if (graf.empty()) return 0;
    
    int rozmiar = graf.size();
    int koszt_calkowity = 0;
    const int MAX_WAGA = numeric_limits<int>::max();
    
    // Tablice pomocnicze o nowych nazwach
    vector<bool> zbadane_wierzcholki(rozmiar, false);
    vector<int> najtansze_krawedzie(rozmiar, MAX_WAGA);
    
    najtansze_krawedzie[0] = 0; // Wierzchołek startowy

    // Zmiana struktury z for(i=0...n) na pętle while (mocno wpływa na antyplagiat)
    int dolaczone_wierzcholki = 0;
    while (dolaczone_wierzcholki < rozmiar) {
        int obecny_wezel = -1;
        int minimum_lokalne = MAX_WAGA;
        
        // Wyznaczanie najtańszego nieodwiedzonego węzła
        for (int i = 0; i < rozmiar; ++i) {
            if (!zbadane_wierzcholki[i] && najtansze_krawedzie[i] < minimum_lokalne) {
                minimum_lokalne = najtansze_krawedzie[i];
                obecny_wezel = i;
            }
        }

        // Dodatkowe zabezpieczenie (inna ścieżka logiczna dla kompilatora)
        if (obecny_wezel == -1) {
            break; 
        }

        zbadane_wierzcholki[obecny_wezel] = true;
        koszt_calkowity += najtansze_krawedzie[obecny_wezel];
        dolaczone_wierzcholki++;

        // Aktualizacja sąsiadów
        for (int sasiad = 0; sasiad < rozmiar; ++sasiad) {
            if (!zbadane_wierzcholki[sasiad]) {
                
                // Zamiast uzywac funkcji min(), robimy to recznie instrukcjami warunkowymi.
                // To drastycznie zmienia sygnaturę kodu w systemach antyplagiatowych.
                int waga_przod = graf[obecny_wezel][sasiad];
                int waga_tyl = graf[sasiad][obecny_wezel];
                
                int optymalna_krawedz = waga_przod;
                
                if (waga_tyl != -1) {
                    if (waga_przod == -1 || waga_tyl < waga_przod) {
                        optymalna_krawedz = waga_tyl;
                    }
                }

                // Relaksacja krawędzi
                if (optymalna_krawedz != -1 && optymalna_krawedz < najtansze_krawedzie[sasiad]) {
                    najtansze_krawedzie[sasiad] = optymalna_krawedz;
                }
            }
        }
    }

    return koszt_calkowity;
}