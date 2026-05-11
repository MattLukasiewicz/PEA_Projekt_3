#ifndef ALGORYTM_MST_H
#define ALGORYTM_MST_H

#include <vector>

// Funkcja wyliczająca Lower Bound (Dolne Ograniczenie) za pomocą algorytmu Prima
int oblicz_LB_MST(const std::vector<std::vector<int>>& graf);

// Funkcja wyliczająca Upper Bound (Górne Ograniczenie) za pomocą Najbliższego Sąsiada (NN)
// Zwraca gotową trasę startową, którą później wrzucimy do Tabu Search
std::vector<int> oblicz_UB_NN(const std::vector<std::vector<int>>& graf);

#endif