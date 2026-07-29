#include "../include/EdgeServer.h"
#include "../include/Metrics.h"
#include <thread>
#include <chrono>

EdgeServer::EdgeServer(OriginServer &origin, Metrics &metrics, size_t cacheCapacity, string loc, int lat)
    : cache(cacheCapacity),
      origin(origin),
      metrics(metrics),
      location(loc),
      latencyMs(lat)
{
}


string EdgeServer::request(const string &key)
{
    // Simulate network latency to this specific edge server
    this_thread::sleep_for(chrono::milliseconds(latencyMs));

    if (cache.contains(key))
    {
        try
        {
            string value = cache.get(key);
            metrics.hit();
            cout << "[EDGE " << location << " (" << latencyMs << "ms)] Cache HIT : " << key << '\n';
            return value;
        }
        catch (const runtime_error &)
        {
            cache.remove(key);
        }
    }

    metrics.miss();
    cout << "[EDGE " << location << " (" << latencyMs << "ms)] Cache MISS : " << key << '\n';

    try
    {
        string data = origin.fetch(key);
        cache.put(key, data, 5); // TTL of 5 seconds
        return data;
    }
    catch (const runtime_error &e)
    {
        cout << "[EDGE " << location << "] ERROR: " << e.what() << '\n';
        return "404 Not Found";
    }
}

void EdgeServer::displayCache() const
{
    cache.display();
}

void EdgeServer::invalidate(const string &key)
{
    cache.remove(key);
    cout << "[EDGE " << location << "] Cache INVALIDATED : " << key << '\n';
}
