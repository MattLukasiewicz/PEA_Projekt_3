#pragma once
#include <vector>
#include <limits>
#include <iostream>

const int INF = std::numeric_limits<int>::max();

class Macierz {
public:
    int rozmiar;
    std::vector<std::vector<int>> dane;
    int optymalnyKoszt;

    Macierz();
    Macierz(int n);

    void wypisz() const;
    int redukuj();
};