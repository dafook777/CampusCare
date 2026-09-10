#include "CampusGraph.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Adds a location to the graph if it doesn't already exist
void CampusGraph::addLocation(const string& name) {
    adj[name]; 
}

// Adds a bidirectional path between two locations with a distance
void CampusGraph::addPath(const string& from, const string& to, int distance) {
    adj[from].push_back({to, distance});
    adj[to].push_back({from, distance});
}

// Displays the complete campus map and its connections
void CampusGraph::displayCampus() const {
    cout << "\n========== CAMPUS GRAPH ==========\n";
    for (const auto& node : adj) {
        cout << left << setw(20) << node.first << " -> ";
        for (const auto& edge : node.second) {
            cout << edge.first << " (" << edge.second << "m) ";
        }
        cout << "\n";
    }
    cout << "==================================\n";
}

// Checks if a location exists in the graph
bool CampusGraph::hasLocation(const string& name) const {
    return adj.find(name) != adj.end();
}
