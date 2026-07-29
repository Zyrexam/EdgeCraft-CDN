#include "../include/OriginServer.h"
#include "../include/EdgeServer.h"
#include "../include/Router.h"
#include <chrono>
#include <thread>

int main()
{
    Metrics metrics;
    OriginServer origin;
    Router router;

    // Wire the Router to the Origin!
    origin.setRouter(&router);

    EdgeServer edge1(origin, metrics, 3);
    EdgeServer edge2(origin, metrics, 3);
    EdgeServer edge3(origin, metrics, 3);

    router.addEdge(&edge1);
    router.addEdge(&edge2);
    router.addEdge(&edge3);

    origin.upload("logo.png", "image data");
    origin.upload("style.css", "body { color: red; }");

    cout << "--- Filling Edge1 Cache ---\n";
    edge1.request("logo.png"); 
    edge1.request("logo.png"); 

    cout << "\n--- UPDATING FILE ON ORIGIN ---\n";
    // uploading a NEW value for logo.png. This should trigger the invalidation!
    origin.upload("logo.png", "NEW image data v2");

    cout << "\n--- Requesting updated file ---\n";
    // Even though Edge1 just had a hit, it should be invalidated now, so it's a miss!
    edge1.request("logo.png"); 

    return 0;
}