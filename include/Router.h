#pragma once
#include <vector>
#include <string>
#include "EdgeServer.h"

using namespace std;

enum class RoutingStrategy
{
    ROUND_ROBIN,
    LEAST_CONNECTIONS,
    CONSISTENT_HASHING
};

class Router
{
private:
    vector<EdgeServer*> edges;
    int currentIndex = 0;
    RoutingStrategy strategy;

public:
    Router(RoutingStrategy strategy = RoutingStrategy::ROUND_ROBIN);

    void addEdge(EdgeServer* edge);

    EdgeServer* getNextEdge(const string &key = ""); // Key is needed for hashing

    string request(const string &key);
    
    void invalidateCache(const string &key);
};