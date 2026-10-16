#ifndef AMBULANCE_DISPATCH_H
#define AMBULANCE_DISPATCH_H

#include "Vehicle.h"
#include "CampusGraph.h"
#include "incident.h"
#include <string>
#include <vector>

class Ambulance : public Vehicle {
private:
    std::string status; // Available, Busy, Offline
    int assignedIncidentId;

public:
    Ambulance(const std::string& ambulanceId = "", const std::string& location = "",
              const std::string& initialStatus = "Available", int incidentId = -1)
        : Vehicle(ambulanceId, location), status(initialStatus), assignedIncidentId(incidentId) {}

    const std::string& getStatus() const { return status; }
    int getAssignedIncidentId() const { return assignedIncidentId; }
    void setStatus(const std::string& newStatus) { status = newStatus; }
    void assignTo(int incidentId) { assignedIncidentId = incidentId; status = "Busy"; }
    void clearAssignment(const std::string& newLocation) {
        currentLocation = newLocation;
        assignedIncidentId = -1;
        status = "Available";
    }
    void display() const override;
};

class AmbulanceDispatch {
private:
    CampusGraph& graph;
    std::vector<Ambulance> ambulances;
    std::vector<Incident>& incidents;
    const std::string ambulanceFile = "ambulances.txt";
    const std::string logFile = "dispatch_log.txt";

    void loadAmbulances();
    void saveAmbulances() const;
    void appendLog(const std::string& message) const;
    Incident* findIncident(int id);
    int findAmbulanceIndex(const std::string& id) const;
    static std::string joinRoute(const std::vector<std::string>& route);

public:
    AmbulanceDispatch(CampusGraph& campusGraph, std::vector<Incident>& incidentList);
    void displayAmbulances() const;
    void dispatchIncident();
    void updateAmbulanceStatus();
    void completeDispatch();
    void displayDispatchLog() const;
};

#endif
