#pragma once
#include <chrono>

class Metrics
{
    long hits = 0;
    long misses = 0;
    long totalRequests = 0;
    std::chrono::steady_clock::time_point startTime;

public:
    Metrics() { startTime = std::chrono::steady_clock::now(); }
    
    void hit();
    void miss();
    double hitRate() const;
    long getTotalRequests() const;
    void display() const;
};