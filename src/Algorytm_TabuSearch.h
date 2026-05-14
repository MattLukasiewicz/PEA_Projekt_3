#ifndef ALGORYTM_TABUSEARCH_H
#define ALGORYTM_TABUSEARCH_H

#include <vector>
#include <string>

// Główna funkcja algorytmu Tabu Search z pełnym sterowaniem
std::vector<int> szukaj_tabu(
    const std::vector<std::vector<int>>& graf, 
    int limit_iteracji, 
    int startowa_kadencja,
    const std::vector<int>& poczatkowa_trasa, // UB z Najbliższego Sąsiada
    int dolne_ograniczenie,                   // LB z MST
    bool uzyj_aspiracji_plus,                 // z configu
    bool uzyj_dynamicznej_kadencji,           // z configu
    const std::string& nazwa_instancji = "",  // <--- NOWE
    const std::string& plik_konwergencji = "" // <--- NOWE
);

#endif