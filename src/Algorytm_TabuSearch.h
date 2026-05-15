#ifndef ALGORYTM_TABUSEARCH_H
#define ALGORYTM_TABUSEARCH_H

#include <vector>
#include <string>

std::vector<int> szukaj_tabu(
    const std::vector<std::vector<int>>& graf, 
    int limit_iteracji, 
    int startowa_kadencja,
    const std::vector<int>& poczatkowa_trasa,
    int dolne_ograniczenie,
    bool uzyj_klasycznej_aspiracji,
    bool uzyj_aspiracji_plus,
    bool uzyj_dynamicznej_kadencji,
    const std::string& nazwa_instancji = "",
    const std::string& plik_konwergencji = ""
);

#endif