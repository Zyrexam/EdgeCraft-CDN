#include "../include/Metrics.h"
#include <iostream>

void Metrics::hit() { hits++; }

void Metrics::miss() { misses++; }

double Metrics::hitRate() const
{
    return (double)hits / (hits + misses);
}

void Metrics::display() const
{
    std::cout << "Hits: " << hits << ", Misses: " << misses
              << ", Hit rate: " << hitRate() * 100 << "%\n";
}