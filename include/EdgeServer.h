#pragma once
#include <string>
#include "Cache.h"
#include "OriginServer.h"
#include "Metrics.h"

using namespace std;

class EdgeServer
{
private:
    Cache cache;
    OriginServer &origin;
    Metrics &metrics;
    string location;       // "Mumbai", "New York"
    int latencyMs;         // Simulated network latency in milliseconds

public:
    EdgeServer(OriginServer &origin, Metrics &metrics, size_t cacheCapacity, string loc, int lat);

    string request(const string &key);
    
    void invalidate(const string &key);

    void displayCache() const;
    
    // Getters for the Router to use
    size_t getCacheSize() const { return cache.size(); }
    string getLocation() const { return location; }
    int getLatencyMs() const { return latencyMs; }
};