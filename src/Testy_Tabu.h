#ifndef TESTY_TABU_H
#define TESTY_TABU_H

#include <string>
#include <vector>
#include "WczytywanieKonfiguracji.h"

// Funkcja przeprowadzająca zautomatyzowane badania
void przeprowadz_testy(const std::string& sciezka_do_pliku, const WczytywanieKonfiguracji& config);

#endif