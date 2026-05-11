#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <limits>

using namespace std;

// Struktura na dane o najlepszym znalezionym sąsiedzie
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
    suma += graf[trasa[n - 1]][trasa[0]]; // domknięcie cyklu
    
    return suma;
}

// Funkcja oceniająca wszystkie możliwe zamiany miast
wynik_ruchu sprawdz_sasiadow(
    const vector<int>& baza,
    const vector<vector<int>>& graf,
    const vector<vector<int>>& zakazy,
    int nr_iteracji,
    int top_koszt,
    int koszt_bazy) 
{
    int n = baza.size();
    wynik_ruchu najlepszy;
    najlepszy.koszt = numeric_limits<int>::max();
    najlepszy.udalo_sie = false;

    // Lecimy po wszystkich parach do zamiany
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            
            vector<int> sasiad = baza;
            swap(sasiad[i], sasiad[j]);
            int nowy_koszt = licz_koszt(sasiad, graf);

            // Zawsze mniejsze miasto pierwsze, zeby nie martwic sie o symetrie
            int m1 = min(baza[i], baza[j]);
            int m2 = max(baza[i], baza[j]);

            // Szybkie sprawdzenie czy ruch ma bana
            bool ruch_zakazany = (zakazy[m1][m2] > nr_iteracji);
            bool klasyczna_aspiracja = (nowy_koszt < top_koszt);

            // Wybór najlepszego ruchu z dozwolonych
            if (!ruch_zakazany || klasyczna_aspiracja) {
                if (nowy_koszt < najlepszy.koszt) {
                    najlepszy.koszt = nowy_koszt;
                    najlepszy.trasa = sasiad;
                    najlepszy.miasto_a = m1;
                    najlepszy.miasto_b = m2;
                    najlepszy.udalo_sie = true;
                }
            }
            // Aspiracja plus: ratujemy sie zablokowanym ruchem, 
            // jesli chociaz troche poprawia nasza beznadziejna sytuacje
            else if (nowy_koszt < koszt_bazy) {
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

// Główna logika optymalizacji
vector<int> szukaj_tabu(const vector<vector<int>>& graf, int limit_iteracji, int startowa_kadencja) {
    int n = graf.size();

    // Na start bierzemy 0, 1, 2... (TUTAJ DODAJ WŁASNE NEAREST NEIGHBOR)
    vector<int> obecna_trasa(n);
    iota(obecna_trasa.begin(), obecna_trasa.end(), 0);
    
    int obecny_koszt = licz_koszt(obecna_trasa, graf);
    
    vector<int> top_trasa = obecna_trasa;
    int top_koszt = obecny_koszt;

    // Macierz tabu z samym -1 na start
    vector<vector<int>> macierz_tabu(n, vector<int>(n, -1));

    // Dynamiczna kadencja uciekajaca ze stagnacji
    int kadencja = startowa_kadencja;
    int brak_zmian = 0;
    
    int iteracja = 0;
    
    while (iteracja < limit_iteracji) {
        
        wynik_ruchu wybrany = sprawdz_sasiadow(
            obecna_trasa, graf, macierz_tabu, iteracja, top_koszt, obecny_koszt
        );

        if (!wybrany.udalo_sie) break;

        obecna_trasa = wybrany.trasa;
        obecny_koszt = wybrany.koszt;

        // Bicie rekordu i modyfikacja kadencji
        if (wybrany.koszt < top_koszt) {
            top_koszt = wybrany.koszt;
            top_trasa = obecna_trasa;
            
            brak_zmian = 0; 
            kadencja = max(2, kadencja - 1); // Luzujemy zakazy
        } 
        else {
            brak_zmian++;
            if (brak_zmian > 50) { 
                kadencja += 2;   // Dokrecamy srube, zeby wyrzucic algorytm z dolka
                brak_zmian = 0;
            }
        }

        // Nakladamy zakaz na obydwa kierunki
        macierz_tabu[wybrany.miasto_a][wybrany.miasto_b] = iteracja + kadencja;
        macierz_tabu[wybrany.miasto_b][wybrany.miasto_a] = iteracja + kadencja;

        iteracja++;
    }

    return top_trasa;
}

int main() {
    // Prosta macierz do testow
    vector<vector<int>> graf = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };

    int max_iter = 1000;
    int poczatkowa_kara = 5;

    vector<int> ostateczna_trasa = szukaj_tabu(graf, max_iter, poczatkowa_kara);

    cout << "Osiagnieto wynik: " << licz_koszt(ostateczna_trasa, graf) << endl;
    cout << "Kolejnosc miast: ";
    for (int miasto : ostateczna_trasa) cout << miasto << " ";
    cout << endl;

    return 0;
}