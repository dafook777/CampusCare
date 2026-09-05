#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <vector>
#include <map>

// ============================================================
// CampusGraph
// Stores campus locations and the paths (weighted edges)
// between them as an adjacency list.
// Dijkstra / BFS / DFS will be added on top of this in Phase 2.
// ============================================================
class CampusGraph {
private:
    std::map<std::string, std::vector<std::pair<std::string, int>>> adj;

public:
    void addLocation(const std::string& name);
    void addPath(const std::string& from, const std::string& to, int distance);
    void displayCampus() const;
    bool hasLocation(const std::string& name) const;
};

#endif // CAMPUS_GRAPH_H