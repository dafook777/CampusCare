#ifndef INCIDENTMANAGER_H
#define INCIDENTMANAGER_H

#include <vector>
#include "incident.h"

using namespace std;

class IncidentManager {
private:
    vector<Incident> incidents;

public:
    IncidentManager();

    void addIncident();
    void displayIncidents();
    void searchIncident();
    void deleteIncident();

    vector<Incident>& getIncidents();
};

#endif