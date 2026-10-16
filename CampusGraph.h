#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <limits>

class CampusGraph {
private:
    std::map<std::string, std::vector<std::pair<std::string, int>>> adj;
    std::set<std::string> hospitals;

public:
    void addLocation(const std::string& name);
    void addPath(const std::string& from, const std::string& to, int distance);
    void addHospital(const std::string& name);
    void displayCampus() const;
    bool hasLocation(const std::string& name) const;
    std::vector<std::string> getLocations() const;

    // Dijkstra's algorithm. Empty path means a location is unknown or unreachable.
    std::vector<std::string> shortestPath(const std::string& start,
                                          const std::string& destination) const;
    int shortestDistance(const std::string& start,
                         const std::string& destination) const;
    bool nearestHospitalRoute(const std::string& start,
                              std::string& hospital,
                              std::vector<std::string>& route,
                              int& distance) const;
};

#endif // CAMPUS_GRAPH_H
