#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <chrono>

using namespace std;

class Cache
{
private:
    struct Node
    {
        string key;
        string value;
        chrono::steady_clock::time_point expiry; // TTL expiry time
        
        Node* prev;
        Node* next;

        Node(string k, string v, chrono::steady_clock::time_point exp)
            : key(k), value(v), expiry(exp), prev(nullptr), next(nullptr) {}
    };

    unordered_map<string, Node*> cacheMap;
    
    Node* head; // head (Most Recently Used)
    Node* tail; // tail (Least Recently Used)
    
    size_t capacity;
    size_t currentSize;

    void addNodeToFront(Node* node);
    void removeNode(Node* node);
    void moveNodeToFront(Node* node);

public:
    Cache(size_t cap);
    
    ~Cache();

    void put(const string &key, const string &value, int ttlSeconds = 0);

    bool contains(const string &key) const;

    string get(const string &key);

    void remove(const string &key);

    void clear();

    size_t size() const;

    void display() const;
};