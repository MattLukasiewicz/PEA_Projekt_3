#include "Testy_Tabu.h"
#include "Wczytywanie_Macierzy.h"
#include "Algorytm_MST.h"
#include "Algorytm_TabuSearch.h"
#include "Stoper.h"
#include "Pasek_postepu.h" // <--- DODANY PASEK
#include <iostream>
#include <fstream>
#include <numeric>

using namespace std;

// Słownik znanych optymów z TSPLIB
int pobierz_optimum(const string& sciezka) {
    if (sciezka.find("br17") != string::npos) return 39;
    if (sciezka.find("ftv33") != string::npos) return 1286;
    if (sciezka.find("ftv35") != string::npos) return 1473;
    if (sciezka.find("ftv38") != string::npos) return 1530;
    if (sciezka.find("ftv47") != string::npos) return 1776;
    if (sciezka.find("p43") != string::npos) return 5620;
    if (sciezka.find("ry48p") != string::npos) return 14422;
    if (sciezka.find("brazil58") != string::npos) return 25395;
    if (sciezka.find("gr48") != string::npos) return 5046;
    return -1;
}

// Funkcja pomocnicza do liczenia kosztu
int policz_koszt(const vector<int>& trasa, const vector<vector<int>>& graf) {
    int suma = 0;
    int n = trasa.size();
    for (int i = 0; i < n - 1; ++i) suma += graf[trasa[i]][trasa[i + 1]];
    suma += graf[trasa[n - 1]][trasa[0]];
    return suma;
}

void przeprowadz_testy(const string& sciezka, const WczytywanieKonfiguracji& config) {
    Macierz macierz = Wczytywanie_Macierzy::wczytajMacierz(sciezka);
    if (macierz.dane.empty()) return;

    vector<vector<int>> graf = macierz.dane;
    int N = graf.size();
    int optimum = pobierz_optimum(sciezka);
    
    ofstream plik_csv(config.plik_wynikowy, ios::app); 
    plik_csv.seekp(0, ios::end);
    if (plik_csv.tellp() == 0) {
        plik_csv << "Plik;N;Optimum;Koszt_TS;Blad_Proc;Czas_ms;Iteracje;Kadencja;UB_On;LB_On;AspiracjaPlus_On;ZmKadencja_On\n";
    }

    Stoper stoper;
    cout << "Rozpoczynam badanie instancji: " << sciezka << " (N=" << N << ")" << endl;

    // Obliczenia do paska postępu
    int calkowita_liczba_testow = 6; // 5 prób kadencji + 1 próba bez UB/LB
    int wykonane_testy = 0;

    // Rysowanie pustego paska na start (0%)
    pokazPostep(config.wyswietlaj_pasek, wykonane_testy, calkowita_liczba_testow, "Testy");

    // =========================================================================
    // SCENARIUSZ 1: Wpływ długości kadencji (5 prób)
    // =========================================================================
    for (int test_kadencji = 5; test_kadencji <= 25; test_kadencji += 5) {
        
        stoper.start(); 
        int LB_prawdziwe = oblicz_LB_MST(graf);
        vector<int> UB_trasa_NN = oblicz_UB_NN(graf);
        
        // Zwróć uwagę: nie przekazujemy już flagi paska do Tabu!
        vector<int> wynik_trasa = szukaj_tabu(
            graf, config.ts_max_iteracji, test_kadencji, 
            UB_trasa_NN, LB_prawdziwe, 
            config.ts_aspiracja_plus, config.ts_zmienna_kadencja
        );
        stoper.stop();
        
        double czas_ms = stoper.pobierzCzasMs();
        int koszt = policz_koszt(wynik_trasa, graf);
        double blad = (optimum > 0) ? ((double)(koszt - optimum) / optimum) * 100.0 : 0.0;

        plik_csv << sciezka << ";" << N << ";" << optimum << ";" << koszt << ";" 
                 << blad << ";" << czas_ms << ";" << config.ts_max_iteracji << ";" 
                 << test_kadencji << ";1;1;" << config.ts_aspiracja_plus << ";" << config.ts_zmienna_kadencja << "\n";

        // Aktualizacja paska po udanym teście!
        wykonane_testy++;
        pokazPostep(config.wyswietlaj_pasek, wykonane_testy, calkowita_liczba_testow, "Testy");
    }

    // =========================================================================
    // SCENARIUSZ 2: Wpływ UB i LB (1 próba)
    // =========================================================================
    stoper.start(); 
    
    vector<int> trasa_naiwna(N);
    iota(trasa_naiwna.begin(), trasa_naiwna.end(), 0);

    vector<int> wynik_bez_ub_lb = szukaj_tabu(
        graf, config.ts_max_iteracji, config.ts_kadencja, 
        trasa_naiwna, -1, 
        config.ts_aspiracja_plus, config.ts_zmienna_kadencja
    );
    
    stoper.stop();
    
    double czas_bez_ms = stoper.pobierzCzasMs();
    int koszt_bez = policz_koszt(wynik_bez_ub_lb, graf);
    double blad_bez = (optimum > 0) ? ((double)(koszt_bez - optimum) / optimum) * 100.0 : 0.0;

    plik_csv << sciezka << ";" << N << ";" << optimum << ";" << koszt_bez << ";" 
             << blad_bez << ";" << czas_bez_ms << ";" << config.ts_max_iteracji << ";" 
             << config.ts_kadencja << ";0;0;" << config.ts_aspiracja_plus << ";" << config.ts_zmienna_kadencja << "\n";

    // Aktualizacja paska po ostatnim teście (100%)
    wykonane_testy++;
    pokazPostep(config.wyswietlaj_pasek, wykonane_testy, calkowita_liczba_testow, "Testy");

    plik_csv.close();
    cout << "Testy zakonczone. Wyniki dopisane do: " << config.plik_wynikowy << endl;
}