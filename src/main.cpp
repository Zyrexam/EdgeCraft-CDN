#include "../include/OriginServer.h"
#include "../include/EdgeServer.h"
#include "../include/Router.h"
#include <random>
#include <cmath>

// Zipfian random number generator
int zipf(double alpha, int n, mt19937& generator) {
    static uniform_real_distribution<double> dist(0.0, 1.0);
    
    // Calculate zeta (harmonic number) for the distribution
    double zeta = 0;
    for (int i = 1; i <= n; i++) {
        zeta += 1.0 / pow(i, alpha);
    }

    double u = dist(generator); // Uniform random number
    double sum = 0;
    
    // Find the Zipfian bucket
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / pow(i, alpha);
        if (sum / zeta >= u) {
            return i - 1; // Return 0-indexed file
        }
    }
    return n - 1;
}

void simulateTraffic(Router& router, OriginServer& origin, int numFiles, int numRequests) {
    // 1. Populate Origin Server
    for (int i = 0; i < numFiles; i++) {
        origin.upload("file_" + to_string(i) + ".txt", "data_" + to_string(i));
    }

    // 2. Setup random generator
    random_device rd;
    mt19937 gen(rd());

    cout << "Simulating " << numRequests << " requests across " << numFiles << " files...\n";

    // 3. Fire requests
    for (int i = 0; i < numRequests; i++) {
        // alpha = 1.2 gives a nice CDN-like curve (some viral files, many obscure files)
        int fileIndex = zipf(1.2, numFiles, gen); 
        string key = "file_" + to_string(fileIndex) + ".txt";
        router.request(key);
    }
}

int main()
{
    Metrics metrics;
    OriginServer origin;

    // Latency set to 0ms for benchmarking!
    EdgeServer mumbaiEdge(origin, metrics, 1000, "Mumbai", 0, true);
    EdgeServer delhiEdge(origin, metrics, 1000, "Delhi", 0, true);
    EdgeServer nyEdge(origin, metrics, 1000, "NewYork", 0, true);

    Router chRouter(RoutingStrategy::CONSISTENT_HASHING);
    chRouter.addEdge(&mumbaiEdge);
    chRouter.addEdge(&delhiEdge);
    chRouter.addEdge(&nyEdge);
    origin.setRouter(&chRouter);

    // Run the simulation: 1000 unique files, 100,000 requests
    simulateTraffic(chRouter, origin, 1000, 100000);

    metrics.display();

    return 0;
}