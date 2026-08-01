#pragma once
#include <vector>
#include <string>
#include <map>
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
    vector<EdgeServer *> edges;
    int currentIndex = 0;
    RoutingStrategy strategy;

    map<size_t, EdgeServer *> ring;
    int numVNodes = 100;

    void addVNodes(EdgeServer *edge);
    void removeVNodes(EdgeServer *edge);
public:
    Router(RoutingStrategy strategy = RoutingStrategy::ROUND_ROBIN);

    void addEdge(EdgeServer *edge);

    void removeEdge(EdgeServer* edge);

    EdgeServer *getNextEdge(const string &key = ""); // Key is needed for hashing

    string request(const string &key);

    void invalidateCache(const string &key);
};