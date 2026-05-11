#include "Testy_Tabu.h"
#include "Wczytywanie_Macierzy.h"
#include "Algorytm_MST.h"
#include "Algorytm_NN_i_RNN.h"
#include "Algorytm_TabuSearch.h"
#include "Stoper.h"
#include "Pasek_postepu.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <limits>

using namespace std;

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
    int optimum = (macierz.optymalnyKoszt > 0) ? macierz.optymalnyKoszt : -1;

    if (optimum == -1) {
        cout << "Uwaga: Brak optimum w pliku (sum_min=...) dla: " << sciezka << endl;
    }

    // Przygotowanie pliku CSV
    ofstream plik_csv(config.plik_wynikowy, ios::app); 
    plik_csv.seekp(0, ios::end);
    if (plik_csv.tellp() == 0) {
        plik_csv << "Plik;N;Optimum;Powtorzenia;Sredni_Koszt;Sredni_Blad_Proc;Sredni_Czas_ms;Iteracje;Kadencja;UB_On;LB_On;AspPlus_On;ZmKadencja_On\n";
    }

    Stoper stoper;
    cout << "Rozpoczynam badanie: " << sciezka << " (N=" << N << "), Powtorzenia: " << config.powtorzenia << endl;

    double suma_czasow_ms = 0.0;
    long long suma_kosztow = 0;
    int najlepszy_znaleziony_koszt = numeric_limits<int>::max();

    pokazPostep(config.wyswietlaj_pasek, 0, config.powtorzenia, "Badanie");

    for (int i = 0; i < config.powtorzenia; i++) {
        
        stoper.start(); 
        
        // Dynamiczne wyliczanie wewnątrz stopera dla rzetelności
        int dolne_ograniczenie = -1;
        if (config.ts_uzyj_lb) {
            dolne_ograniczenie = oblicz_LB_MST(graf);
        }

        vector<int> trasa_startowa;
        if (config.ts_uzyj_ub) {
            if (config.ts_ub_metoda == "RNN") {
                trasa_startowa = oblicz_UB_RNN(graf);
            } else {
                trasa_startowa = oblicz_UB_NN(graf);
            }
        } else {
            trasa_startowa.resize(N);
            iota(trasa_startowa.begin(), trasa_startowa.end(), 0);
        }

        if (static_cast<int>(trasa_startowa.size()) != N) {
            trasa_startowa.resize(N);
            iota(trasa_startowa.begin(), trasa_startowa.end(), 0);
        }

        vector<int> wynik_trasa = szukaj_tabu(
            graf, config.ts_max_iteracji, config.ts_kadencja, 
            trasa_startowa, dolne_ograniczenie, 
            config.ts_aspiracja_plus, config.ts_zmienna_kadencja
        );
        
        stoper.stop();
        
        double czas_ms = stoper.pobierzCzasMs();
        int koszt = policz_koszt(wynik_trasa, graf);
        
        suma_czasow_ms += czas_ms;
        suma_kosztow += koszt;
        if (koszt < najlepszy_znaleziony_koszt) najlepszy_znaleziony_koszt = koszt;

        pokazPostep(config.wyswietlaj_pasek, i + 1, config.powtorzenia, "Badanie");
    }

    // Wyliczanie uśrednionych statystyk
    double sredni_czas = suma_czasow_ms / config.powtorzenia;
    double sredni_koszt = (double)suma_kosztow / config.powtorzenia;
    double sredni_blad = (optimum > 0) ? ((sredni_koszt - optimum) / optimum) * 100.0 : 0.0;

    if (config.wyswietlaj_pasek) cout << "\n";

    cout << "Zakonczono! Najlepszy koszt ze wszystkich prob: " << najlepszy_znaleziony_koszt << "\n";
    cout << "Sredni czas: " << sredni_czas << " ms | Sredni blad: " << sredni_blad << " %\n";

    // Zapis jednym wierszem do pliku CSV
    plik_csv << sciezka << ";" << N << ";" << optimum << ";" << config.powtorzenia << ";" 
             << sredni_koszt << ";" << sredni_blad << ";" << sredni_czas << ";" 
             << config.ts_max_iteracji << ";" << config.ts_kadencja << ";" 
             << config.ts_uzyj_ub << ";" << config.ts_uzyj_lb << ";" 
             << config.ts_aspiracja_plus << ";" << config.ts_zmienna_kadencja << "\n";

    plik_csv.close();
}