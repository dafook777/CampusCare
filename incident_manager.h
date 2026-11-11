#ifndef INCIDENTMANAGER_H
#define INCIDENTMANAGER_H

#include <vector>
#include "incident.h"
#include <unordered_map>
using namespace std;

class IncidentManager {
private:
    vector<Incident> incidents;

    // Hash table for fast searching by incident ID
    unordered_map<int, Incident> incidentHashTable;

public:
    IncidentManager();

    Incident addIncident(const string& ownerId = "");
    void displayIncidents();
    void displayMyReports(const string& ownerId);

    bool updateIncidentStatus(Incident& updatedIncident);

    void searchIncident();
    int deleteIncident();

    void sortIncidentsBySeverity();
    void sortIncidentsById();
    void sortIncidentsByLocation();

    vector<Incident>& getIncidents();
    int getTotalIncidents() const;
    int getPendingIncidents() const;
    int getResolvedIncidents() const;
    vector<int> getSeverityCounts() const;
};

#endif