#include "../include/EdgeServer.h"
#include "../include/Metrics.h"
#include "EdgeServer.h"

EdgeServer::EdgeServer(OriginServer &origin, Metrics &metrics, size_t cacheCapacity)
    : cache(cacheCapacity),
      origin(origin),
      metrics(metrics)
{
}

void EdgeServer::displayCache() const
{
    cache.display();
}

string EdgeServer::request(const string &key)
{
    if (cache.contains(key))
    {
        try
        {
            string value = cache.get(key);
            metrics.hit();
            cout << "[EDGE] Cache HIT : " << key << '\n';
            return value;
        }
        catch (const runtime_error &)
        {
            cache.remove(key);
        }
    }

    metrics.miss();
    cout << "[EDGE] Cache MISS : " << key << '\n';

    string data = origin.fetch(key);

    cache.put(key, data, 5); // Added TTL of 5 seconds for testing!

    return data;
}
void EdgeServer::invalidate(const string &key)
{
    cache.remove(key);
    cout << "[EDGE] Cache INVALIDATED : " << key << '\n';
}
