#include "../include/Router.h"

void Router::addEdge(EdgeServer* edge)
{
    edges.push_back(edge);
}

EdgeServer* Router::getNextEdge()
{
    if (edges.empty()) return nullptr;

    EdgeServer* edge = edges[currentIndex];
    currentIndex = (currentIndex + 1) % edges.size();
    return edge;
}

string Router::request(const string &key)
{
    EdgeServer* edge = getNextEdge();
    if (edge) return edge->request(key);
    return "No edge servers available";
}



void Router::invalidateCache(const string &key)
{
    cout << "[ROUTER] Broadcasting invalidation for: " << key << '\n';
    for (EdgeServer* edge : edges)
    {
        edge->invalidate(key);
    }
}