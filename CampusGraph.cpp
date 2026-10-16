#include "CampusGraph.h"
#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>

using namespace std;

void CampusGraph::addLocation(const string& name) {
    if (!name.empty()) adj[name];
}

void CampusGraph::addPath(const string& from, const string& to, int distance) {
    if (from.empty() || to.empty() || distance < 0) return;
    addLocation(from);
    addLocation(to);

    auto addOrUpdate = [distance](vector<pair<string, int>>& edges, const string& target) {
        for (auto& edge : edges) {
            if (edge.first == target) {
                edge.second = distance;
                return;
            }
        }
        edges.push_back({target, distance});
    };
    addOrUpdate(adj[from], to);
    addOrUpdate(adj[to], from);
}

void CampusGraph::addHospital(const string& name) {
    addLocation(name);
    hospitals.insert(name);
}

void CampusGraph::displayCampus() const {
    cout << "\n========== CAMPUS GRAPH (distances in metres) ==========\n";
    for (const auto& node : adj) {
        cout << left << setw(20) << node.first << " -> ";
        if (node.second.empty()) cout << "no connected paths";
        for (const auto& edge : node.second)
            cout << edge.first << " (" << edge.second << "m) ";
        if (hospitals.find(node.first) != hospitals.end()) cout << " [HOSPITAL]";
        cout << '\n';
    }
    cout << "========================================================\n";
}

bool CampusGraph::hasLocation(const string& name) const {
    return adj.find(name) != adj.end();
}

vector<string> CampusGraph::getLocations() const {
    vector<string> locations;
    for (const auto& node : adj) locations.push_back(node.first);
    return locations;
}

vector<string> CampusGraph::shortestPath(const string& start, const string& destination) const {
    if (!hasLocation(start) || !hasLocation(destination)) return {};
    if (start == destination) return {start};

    const int INF = numeric_limits<int>::max();
    map<string, int> distance;
    map<string, string> previous;
    for (const auto& node : adj) distance[node.first] = INF;
    distance[start] = 0;

    using State = pair<int, string>;
    priority_queue<State, vector<State>, greater<State>> pending;
    pending.push({0, start});

    while (!pending.empty()) {
        State current = pending.top();
        pending.pop();
        int currentDistance = current.first;
        const string& location = current.second;
        if (currentDistance != distance[location]) continue;
        if (location == destination) break;

        for (const auto& edge : adj.at(location)) {
            // Guard against integer overflow while adding distances.
            if (currentDistance > INF - edge.second) continue;
            int candidate = currentDistance + edge.second;
            if (candidate < distance[edge.first]) {
                distance[edge.first] = candidate;
                previous[edge.first] = location;
                pending.push({candidate, edge.first});
            }
        }
    }

    if (distance[destination] == INF) return {}; // disconnected node
    vector<string> path;
    string current = destination;
    while (current != start) {
        path.push_back(current);
        auto it = previous.find(current);
        if (it == previous.end()) return {};
        current = it->second;
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

int CampusGraph::shortestDistance(const string& start, const string& destination) const {
    vector<string> path = shortestPath(start, destination);
    if (path.empty()) return numeric_limits<int>::max();
    int total = 0;
    for (size_t i = 1; i < path.size(); ++i) {
        bool found = false;
        for (const auto& edge : adj.at(path[i - 1])) {
            if (edge.first == path[i]) {
                total += edge.second;
                found = true;
                break;
            }
        }
        if (!found) return numeric_limits<int>::max();
    }
    return total;
}

bool CampusGraph::nearestHospitalRoute(const string& start, string& hospital,
                                      vector<string>& route, int& distance) const {
    if (!hasLocation(start) || hospitals.empty()) return false;
    int bestDistance = numeric_limits<int>::max();
    string bestHospital;
    vector<string> bestRoute;
    for (const string& candidate : hospitals) {
        vector<string> candidateRoute = shortestPath(start, candidate);
        if (candidateRoute.empty()) continue;
        int candidateDistance = shortestDistance(start, candidate);
        if (candidateDistance < bestDistance) {
            bestDistance = candidateDistance;
            bestHospital = candidate;
            bestRoute = candidateRoute;
        }
    }
    if (bestRoute.empty()) return false;
    hospital = bestHospital;
    route = bestRoute;
    distance = bestDistance;
    return true;
}
