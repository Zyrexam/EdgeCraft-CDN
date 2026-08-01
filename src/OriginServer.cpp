#include "../include/OriginServer.h"
#include "../include/Router.h" // router->invalidateCache()

OriginServer::OriginServer() : router(nullptr) {}

void OriginServer::setRouter(Router *r)
{
    router = r;
}

void OriginServer::upload(const string &key, const string &value)
{

    bool isUpdate = storage.find(key) != storage.end();

    storage[key] = value;

    if (isUpdate && router != nullptr)
    {
        router->invalidateCache(key);
    }
}

bool OriginServer::exists(const string &key) const
{
    return storage.find(key) != storage.end();
}

string OriginServer::fetch(const string &key) const
{
    auto it = storage.find(key);

    if (it == storage.end())
        throw runtime_error("Origin: File not found.");

    return it->second;
}

void OriginServer::remove(const string &key)
{
    storage.erase(key);
}

void OriginServer::display() const
{
    for (const auto &[key, value] : storage)
    {
        cout << key << " -> " << value << '\n';
    }
}
