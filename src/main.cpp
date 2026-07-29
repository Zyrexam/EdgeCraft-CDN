#include "../include/OriginServer.h"
#include "../include/EdgeServer.h"
#include "../include/Router.h"
#include <chrono>
#include <thread>

int main()
{
    Metrics metrics;
    OriginServer origin;

    // Create Edge Servers with simulated locations and latencies
    EdgeServer mumbaiEdge(origin, metrics, 100, "Mumbai", 5);
    EdgeServer delhiEdge(origin, metrics, 100, "Delhi", 20);
    EdgeServer nyEdge(origin, metrics, 100, "New York", 200);

    origin.upload("logo.png", "image data");

    // TEST 1: Round Robin

    cout << "==== TEST 1: ROUND ROBIN ====\n";
    Router rrRouter(RoutingStrategy::ROUND_ROBIN);
    rrRouter.addEdge(&mumbaiEdge);
    rrRouter.addEdge(&delhiEdge);
    rrRouter.addEdge(&nyEdge);
    origin.setRouter(&rrRouter);

    for (int i = 0; i < 6; i++) {
        rrRouter.request("logo.png");
    }

    cout << "\n";




    // Consistent Hashing
    cout << "==== TEST 2: CONSISTENT HASHING ====\n";
    
    // Clear the caches from the previous test to start fresh
    mumbaiEdge.invalidate("logo.png");
    delhiEdge.invalidate("logo.png");
    nyEdge.invalidate("logo.png");

    Router chRouter(RoutingStrategy::CONSISTENT_HASHING);
    chRouter.addEdge(&mumbaiEdge);
    chRouter.addEdge(&delhiEdge);
    chRouter.addEdge(&nyEdge);
    origin.setRouter(&chRouter);

    for (int i = 0; i < 6; i++) {
        chRouter.request("logo.png");
    }

    cout << "\n==== Final Metrics ====\n";
    metrics.display();

    return 0;
}