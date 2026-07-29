#include "../include/Cache.h"
#include "Cache.h"

Cache::Cache(size_t cap) : capacity(cap), currentSize(0)
{
    head = new Node("", "", chrono::steady_clock::now());
    tail = new Node("", "", chrono::steady_clock::now());

    head->next = tail;
    tail->prev = head;
}

Cache::~Cache()
{
    Node *current = head->next;
    while (current != tail)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
    delete head;
    delete tail;
}

void Cache::addNodeToFront(Node *node)
{
    node->next = head->next;
    node->prev = head;

    head->next->prev = node;
    head->next = node;
}

void Cache::removeNode(Node *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void Cache::moveNodeToFront(Node *node)
{
    removeNode(node);
    addNodeToFront(node);
}

string Cache::get(const string &key) 
{
    auto it = cacheMap.find(key);

    if (it == cacheMap.end())
    {
        throw runtime_error("Cache Miss: " + key);
    }

    Node *node = it->second;

    if (node->expiry != chrono::steady_clock::time_point() &&
        chrono::steady_clock::now() > node->expiry)
    {
        throw runtime_error("Cache Miss (Expired): " + key);
    }

    const_cast<Cache *>(this)->moveNodeToFront(node);

    return node->value;
}

void Cache::put(const string &key, const string &value, int ttlSeconds)
{
    auto it = cacheMap.find(key);

    // Calculate expiry time
    chrono::steady_clock::time_point expiry = chrono::steady_clock::time_point();
    if (ttlSeconds > 0)
    {
        expiry = chrono::steady_clock::now() + chrono::seconds(ttlSeconds);
    }

    if (it != cacheMap.end())
    {
        // Key exists: Update value and move to front
        Node *node = it->second;
        node->value = value;
        node->expiry = expiry;
        moveNodeToFront(node);
    }
    else
    {
        // Key doesn't exist: Create new node

        // Check if cache is full
        if (currentSize == capacity)
        {
            // Evict LRU node (the one right before tail)
            Node *lru = tail->prev;
            removeNode(lru);
            cacheMap.erase(lru->key);
            delete lru;
            currentSize--;
        }

        // Add new node
        Node *newNode = new Node(key, value, expiry);
        addNodeToFront(newNode);
        cacheMap[key] = newNode;
        currentSize++;
    }
}

bool Cache::contains(const string &key) const
{
    return cacheMap.find(key) != cacheMap.end();
}

void Cache::remove(const string &key)
{
    auto it = cacheMap.find(key);
    if (it != cacheMap.end())
    {
        Node *node = it->second;
        removeNode(node);
        delete node;
        cacheMap.erase(it);
        currentSize--;
    }
}

void Cache::clear()
{
    Node *current = head->next;
    while (current != tail)
    {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head->next = tail;
    tail->prev = head;
    cacheMap.clear();
    currentSize = 0;
}

size_t Cache::size() const
{
    return currentSize;
}

void Cache::display() const
{
    if (cacheMap.empty())
    {
        cout << "Cache is empty\n";
        return;
    }

    Node *current = head->next;
    while (current != tail)
    {
        cout << current->key << " -> " << current->value << '\n';
        current = current->next;
    }
}