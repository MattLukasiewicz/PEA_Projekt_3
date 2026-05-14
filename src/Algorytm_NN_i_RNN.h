#pragma once
#include "Macierze.h"
#include <vector>

int algorytm_nn(const Macierz& macierz);

int algorytm_rnn(const Macierz& macierz);

int rozwiazAlgorytm_NN(const Macierz& macierz);

std::vector<int> oblicz_UB_NN(const std::vector<std::vector<int>>& graf);

std::vector<int> oblicz_UB_RNN(const std::vector<std::vector<int>>& graf);

struct RnnWynik {
	int koszt;
	std::vector<int> trasa;
};