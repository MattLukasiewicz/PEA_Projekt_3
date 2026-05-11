#include "Stoper.h"

using namespace std;

void Stoper::start() {
    czas_start = chrono::high_resolution_clock::now();
}

void Stoper::stop() {
    czas_stop = chrono::high_resolution_clock::now();
}

double Stoper::pobierzCzasMs() {
    chrono::duration<double, milli> czas_wykonania = czas_stop - czas_start;
    return czas_wykonania.count();
}