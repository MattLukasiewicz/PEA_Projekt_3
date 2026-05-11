#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

inline std::string zbudujPasekPostepu(long long wykonane, long long lacznie) {
    const int szerokoscPaska = 30;
    const long long bezpieczneLacznie = std::max(1LL, lacznie);
    const long long bezpieczneWykonane = std::max(0LL, std::min(wykonane, bezpieczneLacznie));
    const double procent = (100.0 * static_cast<double>(bezpieczneWykonane)) / static_cast<double>(bezpieczneLacznie);
    const int wypelnienie = static_cast<int>((szerokoscPaska * bezpieczneWykonane) / bezpieczneLacznie);

    std::ostringstream out;
    out << "[";
    for (int i = 0; i < szerokoscPaska; ++i) {
        out << (i < wypelnienie ? '#' : '-');
    }
    out << "] " << std::fixed << std::setprecision(1) << procent << "% ("
        << bezpieczneWykonane << "/" << bezpieczneLacznie << ")";

    return out.str();
}

inline void pokazPostep(bool wlaczony, long long wykonane, long long lacznie, const std::string& etykieta) {
    if (!wlaczony) {
        return;
    }

    const int szerokoscPaska = 30;
    const long long bezpieczneLacznie = std::max(1LL, lacznie);
    const long long bezpieczneWykonane = std::max(0LL, std::min(wykonane, bezpieczneLacznie));
    const double procent = (100.0 * static_cast<double>(bezpieczneWykonane)) / static_cast<double>(bezpieczneLacznie);
    const int wypelnienie = static_cast<int>((szerokoscPaska * bezpieczneWykonane) / bezpieczneLacznie);

    std::cout << "\r" << etykieta << " [";
    for (int i = 0; i < szerokoscPaska; ++i) {
        std::cout << (i < wypelnienie ? '#' : '-');
    }

    std::cout << "] " << std::fixed << std::setprecision(1) << procent
              << "% (" << bezpieczneWykonane << "/" << bezpieczneLacznie << ")" << std::flush;

    if (bezpieczneWykonane >= bezpieczneLacznie) {
        std::cout << "\n";
    }
}