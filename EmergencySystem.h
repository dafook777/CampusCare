
#ifndef EMERGENCYSYSTEM_H
#define EMERGENCYSYSTEM_H

#include <queue>
#include <vector>
#include <set>
#include "incident.h"

using namespace std;

struct IncidentPriority {
    bool operator()(const Incident& a, const Incident& b) const {
        if (a.severity != b.severity)
            return a.severity < b.severity;

        return a.id > b.id;
    }
};

class EmergencySystem {

private:
    queue<Incident> normalQueue;

    priority_queue<
        Incident,
        vector<Incident>,
        IncidentPriority
    > emergencyPriorityQueue;

    set<int> processedIncidents;
    void loadProcessedIncidents();
    void saveProcessedIncidents() const;

public:

    // Empty emergency system
    EmergencySystem();

    // Create emergency system using saved incidents
    EmergencySystem(const vector<Incident>& existingIncidents);

    void addIncident(const Incident& incident);

    // NEW: Remove a deleted incident from both queues
    void removeIncident(int incidentId);

    void showNormalQueue();

    void showPriorityQueue();

    void processNextNormal();

    void processHighestPriority();

};

#endif
