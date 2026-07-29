#include "../include/Router.h"
#include <functional>
#include <algorithm>

Router::Router(RoutingStrategy strat) : strategy(strat) {}

void Router::addEdge(EdgeServer *edge)
{
    edges.push_back(edge);
}

EdgeServer *Router::getNextEdge(const string &key)
{
    if (edges.empty())
        return nullptr;

    if (strategy == RoutingStrategy::ROUND_ROBIN)
    {

        EdgeServer *edge = edges[currentIndex];
        currentIndex = (currentIndex + 1) % edges.size();
        return edge;
    }

    else if (strategy == RoutingStrategy::LEAST_CONNECTIONS)
    {
        // Find the edge server with the least items in its cache
        EdgeServer *bestEdge = edges[0];
        for (auto edge : edges)
        {
            if (edge->getCacheSize() < bestEdge->getCacheSize())
            {
                bestEdge = edge;
            }
        }
        return bestEdge;
    }

    else if (strategy == RoutingStrategy::CONSISTENT_HASHING)
    {
        // Hash the key and modulo by the number of edges
        hash<string> hashFn;
        size_t hashValue = hashFn(key);
        size_t index = hashValue % edges.size();
        return edges[index];
    }

    return nullptr; // Fallback
}

string Router::request(const string &key)
{
    EdgeServer *edge = getNextEdge();
    if (edge)
        return edge->request(key);
    return "No edge servers available";
}

void Router::invalidateCache(const string &key)
{
    cout << "[ROUTER] Broadcasting invalidation for: " << key << '\n';
    for (EdgeServer *edge : edges)
    {
        edge->invalidate(key);
    }
}
