#pragma once

class Metrics
{
    long hits = 0;
    long misses = 0;

public:
    void hit();
    void miss();
    double hitRate() const;
    void display() const;
};