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

