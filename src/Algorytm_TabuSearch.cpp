#include "Algorytm_TabuSearch.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

struct Zakaz {
    int miasto_a;
    int miasto_b;
};

struct wynik_ruchu {
    vector<int> trasa;
    int koszt;
    int miasto_a;
    int miasto_b;
    bool udalo_sie;
};

int licz_koszt(const vector<int>& trasa, const vector<vector<int>>& graf) {
    int suma = 0;
    int n = trasa.size();
    for (int i = 0; i < n - 1; ++i) {
        suma += graf[trasa[i]][trasa[i + 1]];
    }
    suma += graf[trasa[n - 1]][trasa[0]];
    return suma;
}

wynik_ruchu sprawdz_sasiadow(
    const vector<int>& baza,
    const vector<vector<int>>& graf,
    const vector<Zakaz>& lista_tabu,
    int top_koszt,
    int koszt_bazy,
    bool uzyj_klasycznej_aspiracji,
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

            bool ruch_zakazany = false;
            for (size_t k = 0; k < lista_tabu.size(); ++k) {
                if (lista_tabu[k].miasto_a == m1 && lista_tabu[k].miasto_b == m2) {
                    ruch_zakazany = true;
                    break;
                }
            }

            bool klasyczna_aspiracja = uzyj_klasycznej_aspiracji && (nowy_koszt < top_koszt);

            if (!ruch_zakazany || klasyczna_aspiracja) {
                if (nowy_koszt < najlepszy.koszt) {
                    najlepszy.koszt = nowy_koszt;
                    najlepszy.trasa = sasiad;
                    najlepszy.miasto_a = m1;
                    najlepszy.miasto_b = m2;
                    najlepszy.udalo_sie = true;
                }
            }
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

vector<int> szukaj_tabu(
    const vector<vector<int>>& graf, 
    int limit_iteracji, 
    int startowa_dlugosc_listy,
    const vector<int>& poczatkowa_trasa,
    int dolne_ograniczenie,
    bool uzyj_klasycznej_aspiracji,
    bool uzyj_aspiracji_plus,
    bool uzyj_dynamicznej_kadencji,
    const string& nazwa_instancji,
    const string& plik_konwergencji) 
{
    vector<int> obecna_trasa = poczatkowa_trasa;
    int obecny_koszt = licz_koszt(obecna_trasa, graf);
    
    vector<int> top_trasa = obecna_trasa;
    int top_koszt = obecny_koszt;

    vector<Zakaz> lista_tabu;
    int dlugosc_listy = startowa_dlugosc_listy;

    int brak_zmian = 0;
    int iteracja = 0;
    
    if (!plik_konwergencji.empty()) {
        ofstream plik(plik_konwergencji, ios::app);
        plik << nazwa_instancji << ";" << 0 << ";" << top_koszt << "\n";
    }
    
    while (iteracja < limit_iteracji) {
        
        if (top_koszt == dolne_ograniczenie) {
            cout << "Tabu Search zeszlo do udowodnionego minimum (LB)! Koniec szukania w iteracji " << iteracja << ".\n";
            break;
        }

        wynik_ruchu wybrany = sprawdz_sasiadow(
            obecna_trasa,
            graf,
            lista_tabu,
            top_koszt,
            obecny_koszt,
            uzyj_klasycznej_aspiracji,
            uzyj_aspiracji_plus
        );

        if (!wybrany.udalo_sie) break;

        obecna_trasa = wybrany.trasa;
        obecny_koszt = wybrany.koszt;

        if (wybrany.koszt < top_koszt) {
            top_koszt = wybrany.koszt;
            top_trasa = obecna_trasa;
            
            if (!plik_konwergencji.empty()) {
                ofstream plik(plik_konwergencji, ios::app);
                plik << nazwa_instancji << ";" << iteracja << ";" << top_koszt << "\n";
            }

            brak_zmian = 0; 
            if (uzyj_dynamicznej_kadencji) {
                dlugosc_listy = max(2, dlugosc_listy - 1);
            }
        } 
        else {
            brak_zmian++;
            if (uzyj_dynamicznej_kadencji && brak_zmian > 50) { 
                dlugosc_listy += 2;
                brak_zmian = 0;
            }
        }

        Zakaz nowy_zakaz;
        nowy_zakaz.miasto_a = wybrany.miasto_a;
        nowy_zakaz.miasto_b = wybrany.miasto_b;
        lista_tabu.push_back(nowy_zakaz);

        while (lista_tabu.size() > static_cast<size_t>(dlugosc_listy)) {
            lista_tabu.erase(lista_tabu.begin());
        }

        iteracja++;
    }

    return top_trasa;
}