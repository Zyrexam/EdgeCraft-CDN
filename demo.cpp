#include "../include/EdgeServer.h"
#include "../include/Metrics.h"
#include <thread>
#include <chrono>

EdgeServer::EdgeServer(OriginServer &origin, Metrics &metrics, size_t cacheCapacity, string loc, int lat, bool silent)
    : cache(cacheCapacity), 
      origin(origin),
      metrics(metrics),
      location(loc),
      latencyMs(lat),
      silentMode(silent) // <-- ADD THIS
{
}

string EdgeServer::request(const string &key)
{
    this_thread::sleep_for(chrono::milliseconds(latencyMs));

    if (cache.contains(key))
    {
        try {
            string value = cache.get(key);
            metrics.hit();
            if (!silentMode) cout << "[EDGE " << location << " (" << latencyMs << "ms)] Cache HIT : " << key << '\n';
            return value;
        }
        catch (const runtime_error &) {
            cache.remove(key); 
        }
    }

    metrics.miss();
    if (!silentMode) cout << "[EDGE " << location << " (" << latencyMs << "ms)] Cache MISS : " << key << '\n';

    try {
        string data = origin.fetch(key);
        cache.put(key, data, 5); 
        return data;
    }
    catch (const runtime_error& e) {
        if (!silentMode) cout << "[EDGE " << location << "] ERROR: " << e.what() << '\n';
        return "404 Not Found";
    }
}

void EdgeServer::invalidate(const string &key)
{
    cache.remove(key);
    if (!silentMode) cout << "[EDGE " << location << "] Cache INVALIDATED : " << key << '\n';
}

void EdgeServer::displayCache() const
{
    cout << "--- Cache at " << location << " ---\n";
    cache.display();
}