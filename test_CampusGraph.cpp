
#include "CampusGraph.h"
#include <cassert>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

int main() {
    CampusGraph graph;

    // Create campus locations.
    graph.addLocation("Main Gate");
    graph.addLocation("Library");
    graph.addLocation("Cafeteria");
    graph.addLocation("Medical Center");
    graph.addLocation("Hostel");

    // Create paths with distances in metres.
    graph.addPath("Main Gate", "Library", 100);
    graph.addPath("Library", "Medical Center", 200);
    graph.addPath("Main Gate", "Cafeteria", 50);
    graph.addPath("Cafeteria", "Medical Center", 100);

    // Register hospitals.
    graph.addHospital("Cafeteria");
    graph.addHospital("Medical Center");

    // Test 1: Shortest route.
    vector<string> route =
        graph.shortestPath("Main Gate", "Medical Center");

    assert((route == vector<string>{
        "Main Gate", "Cafeteria", "Medical Center"
    }));

    assert(graph.shortestDistance("Main Gate", "Medical Center") == 150);

    cout << "Test 1 passed: shortest route is correct.\n";

    // Test 2: Route from a location to itself.
    assert(graph.shortestPath("Main Gate", "Main Gate")
           == vector<string>{"Main Gate"});

    assert(graph.shortestDistance("Main Gate", "Main Gate") == 0);

    cout << "Test 2 passed: same-location route is correct.\n";

    // Test 3: Unknown destination.
    assert(graph.shortestPath("Main Gate", "Unknown").empty());

    cout << "Test 3 passed: unknown location is handled.\n";

    // Test 4: Disconnected location.
    assert(graph.shortestDistance("Main Gate", "Hostel")
           == numeric_limits<int>::max());

    cout << "Test 4 passed: disconnected location is handled.\n";

    // Test 5: Nearest reachable hospital.
    string hospital;
    vector<string> hospitalRoute;
    int hospitalDistance = 0;

    bool found = graph.nearestHospitalRoute(
        "Main Gate", hospital, hospitalRoute, hospitalDistance
    );

    assert(found);
    assert(hospital == "Cafeteria");
    assert(hospitalDistance == 50);

    cout << "Test 5 passed: nearest hospital is correct.\n";

    cout << "\nAll graph tests passed!\n";

    return 0;
}
