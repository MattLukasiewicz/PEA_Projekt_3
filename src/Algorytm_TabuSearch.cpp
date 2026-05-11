#include "Algorytm_TabuSearch.h"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

// Struktura na dane o najlepszym ruchu
struct wynik_ruchu {
    vector<int> trasa;
    int koszt;
    int miasto_a;
    int miasto_b;
    bool udalo_sie;
};

// Funkcja do szybkiego policzenia sumy odległości
int licz_koszt(const vector<int>& trasa, const vector<vector<int>>& graf) {
    int suma = 0;
    int n = trasa.size();
    for (int i = 0; i < n - 1; ++i) {
        suma += graf[trasa[i]][trasa[i + 1]];
    }
    suma += graf[trasa[n - 1]][trasa[0]];
    return suma;
}

// Funkcja oceniająca sąsiedztwo
wynik_ruchu sprawdz_sasiadow(
    const vector<int>& baza,
    const vector<vector<int>>& graf,
    const vector<vector<int>>& zakazy,
    int nr_iteracji,
    int top_koszt,
    int koszt_bazy,
    bool uzyj_aspiracji_plus) 
{
    int n = baza.size();
    wynik_ruchu najlepszy;
    najlepszy.koszt = numeric_limits<int>::max();
    najlepszy.udalo_sie = false;

    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            
            vector<int> sasiad = baza;
            swap(sasiad[i], sasiad[j]);
            int nowy_koszt = licz_koszt(sasiad, graf);

            int m1 = min(baza[i], baza[j]);
            int m2 = max(baza[i], baza[j]);

            bool ruch_zakazany = (zakazy[m1][m2] > nr_iteracji);
            bool klasyczna_aspiracja = (nowy_koszt < top_koszt);

            if (!ruch_zakazany || klasyczna_aspiracja) {
                if (nowy_koszt < najlepszy.koszt) {
                    najlepszy.koszt = nowy_koszt;
                    najlepszy.trasa = sasiad;
                    najlepszy.miasto_a = m1;
                    najlepszy.miasto_b = m2;
                    najlepszy.udalo_sie = true;
                }
            }
            // Aspiracja plus (jeśli włączona w configu)
            else if (uzyj_aspiracji_plus && (nowy_koszt < koszt_bazy)) {
                if (nowy_koszt < najlepszy.koszt) {
                    najlepszy.koszt = nowy_koszt;
                    najlepszy.trasa = sasiad;
                    najlepszy.miasto_a = m1;
                    najlepszy.miasto_b = m2;
                    najlepszy.udalo_sie = true;
                }
            }
        }
    }
    return najlepszy;
}

// Główna funkcja Tabu
vector<int> szukaj_tabu(
    const vector<vector<int>>& graf, 
    int limit_iteracji, 
    int startowa_kadencja,
    const vector<int>& poczatkowa_trasa,
    int dolne_ograniczenie,
    bool uzyj_aspiracji_plus,
    bool uzyj_dynamicznej_kadencji) 
{
    int n = graf.size();

    // 1. Używamy trasy z algorytmu NN zamiast naiwnej listy 0,1,2...
    vector<int> obecna_trasa = poczatkowa_trasa;
    int obecny_koszt = licz_koszt(obecna_trasa, graf);
    
    vector<int> top_trasa = obecna_trasa;
    int top_koszt = obecny_koszt;

    vector<vector<int>> macierz_tabu(n, vector<int>(n, -1));

    int kadencja = startowa_kadencja;
    int brak_zmian = 0;
    int iteracja = 0;
    
    while (iteracja < limit_iteracji) {
        
        // 2. PRZEDWCZESNY STOP: Sprawdzenie Lower Bound!
        if (top_koszt == dolne_ograniczenie) {
            cout << " -> Tabu Search zeszlo do udowodnionego minimum (LB)! Koniec szukania w iteracji " << iteracja << ".\n";
            break;
        }

        wynik_ruchu wybrany = sprawdz_sasiadow(
            obecna_trasa, graf, macierz_tabu, iteracja, top_koszt, obecny_koszt, uzyj_aspiracji_plus
        );

        if (!wybrany.udalo_sie) break; // Zabezpieczenie przed totalnym zakleszczeniem

        obecna_trasa = wybrany.trasa;
        obecny_koszt = wybrany.koszt;

        if (wybrany.koszt < top_koszt) {
            top_koszt = wybrany.koszt;
            top_trasa = obecna_trasa;
            
            brak_zmian = 0; 
            if (uzyj_dynamicznej_kadencji) {
                kadencja = max(2, kadencja - 1); // Zmniejszamy restrykcje, gdy idzie dobrze
            }
        } 
        else {
            brak_zmian++;
            if (uzyj_dynamicznej_kadencji && brak_zmian > 50) { 
                kadencja += 2;   // Ucieczka z lokalnego minimum
                brak_zmian = 0;
            }
        }

        macierz_tabu[wybrany.miasto_a][wybrany.miasto_b] = iteracja + kadencja;
        macierz_tabu[wybrany.miasto_b][wybrany.miasto_a] = iteracja + kadencja;

        iteracja++;
    }

    return top_trasa;
}