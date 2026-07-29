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

public:
    EdgeServer(OriginServer &origin, Metrics &metrics, size_t cacheCapacity = 3);

    string request(const string &key);

    void displayCache() const;

    void invalidate(const string &key); 
};