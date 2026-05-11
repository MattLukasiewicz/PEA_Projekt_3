#include "Algorytm_MST.h"
#include <limits>
#include <algorithm>

using namespace std;

// --- OBLICZANIE LOWER BOUND (Algorytm Prima - Minimalne Drzewo Rozpinające) ---
int oblicz_LB_MST(const vector<vector<int>>& graf) {
    int n = graf.size();
    if (n == 0) return 0;

    vector<bool> w_drzewie(n, false);
    vector<int> min_koszt(n, numeric_limits<int>::max());
    
    min_koszt[0] = 0; // Zaczynamy od wierzcholka 0
    int suma_mst = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1;
        
        // Szukamy wierzchołka o najmniejszym koszcie, którego nie ma w drzewie
        for (int j = 0; j < n; ++j) {
            if (!w_drzewie[j] && (u == -1 || min_koszt[j] < min_koszt[u])) {
                u = j;
            }
        }

        w_drzewie[u] = true;
        suma_mst += min_koszt[u];

        // Aktualizujemy koszty sąsiadów
        for (int v = 0; v < n; ++v) {
            // Zabezpieczenie dla ATSP: bierzemy najmniejszą z dwóch dróg (u->v lub v->u)
            int koszt_krawedzi = min(graf[u][v], graf[v][u]); 
            
            if (graf[u][v] != -1 && !w_drzewie[v] && koszt_krawedzi < min_koszt[v]) {
                min_koszt[v] = koszt_krawedzi;
            }
        }
    }

    return suma_mst;
}

// --- OBLICZANIE UPPER BOUND (Algorytm Najbliższego Sąsiada - NN) ---
vector<int> oblicz_UB_NN(const vector<vector<int>>& graf) {
    int n = graf.size();
    vector<int> trasa;
    if (n == 0) return trasa;

    vector<bool> odwiedzone(n, false);
    
    int obecny = 0; // Zaczynamy z miasta 0
    trasa.push_back(obecny);
    odwiedzone[obecny] = true;

    for (int i = 1; i < n; ++i) {
        int nastepny = -1;
        int min_dystans = numeric_limits<int>::max();

        // Szukamy najbliższego nieodwiedzonego sąsiada
        for (int j = 0; j < n; ++j) {
            if (!odwiedzone[j] && graf[obecny][j] < min_dystans) {
                min_dystans = graf[obecny][j];
                nastepny = j;
            }
        }

        obecny = nastepny;
        trasa.push_back(obecny);
        odwiedzone[obecny] = true;
    }

    return trasa;
}