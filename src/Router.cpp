#include "../include/Router.h"
#include <functional>
#include <algorithm>

Router::Router(RoutingStrategy strat) : strategy(strat) {}

void Router::addVNodes(EdgeServer *edge)
{
    hash<string> hashfn;

    for (int i = 0; i < numVNodes; i++)
    {
        string vNodeName = edge->getLocation() + "_VNode" + to_string(i);
        size_t hashValue = hashfn(vNodeName);
        ring[hashValue] = edge;
    }
}

void Router::removeVNodes(EdgeServer *edge)
{
    hash<string> hashFn;
    for (int i = 0; i < numVNodes; i++)
    {
        string vNodeName = edge->getLocation() + "_VNode_" + to_string(i);
        size_t hashValue = hashFn(vNodeName);
        ring.erase(hashValue);
    }
}

void Router::addEdge(EdgeServer *edge)
{
    edges.push_back(edge);

    if (strategy == RoutingStrategy::CONSISTENT_HASHING)
    {
        addVNodes(edge);
    }
}

void Router::removeEdge(EdgeServer *edge)
{

    auto it = find(edges.begin(), edges.end(), edge);
    if (it != edges.end())
        edges.erase(it);

    if (strategy == RoutingStrategy::CONSISTENT_HASHING)
    {
        removeVNodes(edge);
    }
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

        hash<string> hashFn;
        size_t hashValue = hashFn(key);

        auto it = ring.upper_bound(hashValue);

        if (it == ring.end())
        {
            it = ring.begin();
        }

        return it->second;
    }

    return nullptr;
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
