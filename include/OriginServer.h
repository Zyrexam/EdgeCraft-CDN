#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class Router; // <-- FORWARD DECLARATION: To compiler, Router exists

class OriginServer
{
private:
    unordered_map<string, string> storage;
    Router* router;

public:
    OriginServer();

    void setRouter(Router* r);

    void upload(const string& key, const string& value);

    bool exists(const string& key) const;

    string fetch(const string& key) const;

    void remove(const string& key);

    void display() const;
};
