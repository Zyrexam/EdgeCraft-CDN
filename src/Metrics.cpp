#include "../include/Metrics.h"
#include <iostream>

void Metrics::hit() { hits++; totalRequests++; }
void Metrics::miss() { misses++; totalRequests++; }

double Metrics::hitRate() const
{
    if (hits + misses == 0) return 0.0;
    return (double)hits / (hits + misses);
}

long Metrics::getTotalRequests() const { return totalRequests; }

void Metrics::display() const
{
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    double seconds = duration / 1000.0;
    double rps = (seconds > 0) ? (totalRequests / seconds) : 0;

    std::cout << "      ZIPF'S SIMULATION METRICS         \n";
    std::cout << "Total Requests : " << totalRequests << "\n";
    std::cout << "Cache Hits     : " << hits << "\n";
    std::cout << "Cache Misses   : " << misses << "\n";
    std::cout << "Hit Rate       : " << hitRate() * 100 << "%\n";
    std::cout << "Throughput     : " << rps << " req/sec\n";
 
}