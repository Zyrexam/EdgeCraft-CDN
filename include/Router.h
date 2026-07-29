#pragma once
#include <vector>
#include <string>
#include "EdgeServer.h"

using namespace std;

class Router
{
private:
    vector<EdgeServer*> edges;
    int currentIndex = 0;

public:
    void addEdge(EdgeServer* edge);

    EdgeServer* getNextEdge();

    string request(const string &key);

    void invalidateCache(const string &key);
};
