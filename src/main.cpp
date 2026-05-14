#include <iostream>
#include "WczytywanieKonfiguracji.h"
#include "Testy_Tabu.h"

using namespace std;

int main() {
    WczytywanieKonfiguracji config;
    if (!config.wczytaj_z_pliku("config.txt")) {
        return 1;
    }

    cout << "PEA Projekt 3: Tabu Search" << endl;
    
    przeprowadz_testy(config.sciezka_do_pliku, config);

    return 0;
}