#pragma once
#include <chrono>

class Stoper {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> czas_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> czas_stop;

public:
    void start();
    void stop();
    double pobierzCzasMs();
};