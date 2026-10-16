#include "AmbulanceDispatch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

void Ambulance::display() const {
    cout << "Ambulance ID : " << id << '\n'
         << "Location     : " << currentLocation << '\n'
         << "Status       : " << status << '\n';
    if (assignedIncidentId != -1) cout << "Assigned to  : Incident #" << assignedIncidentId << '\n';
    cout << "----------------------------------------\n";
}

AmbulanceDispatch::AmbulanceDispatch(CampusGraph& campusGraph, vector<Incident>& incidentList)
    : graph(campusGraph), incidents(incidentList) {
    loadAmbulances();
}

void AmbulanceDispatch::loadAmbulances() {
    ifstream file(ambulanceFile);
    if (file) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream row(line);
            string id, location, status, incidentText;
            if (getline(row, id, '|') && getline(row, location, '|') &&
                getline(row, status, '|') && getline(row, incidentText)) {
                try {
                    int assigned = stoi(incidentText);
                    if (graph.hasLocation(location) &&
                        (status == "Available" || status == "Busy" || status == "Offline")) {
                        ambulances.emplace_back(id, location, status, assigned);
                    }
                } catch (...) { /* Ignore malformed data rows. */ }
            }
        }
    }

    if (ambulances.empty()) {
        // Demo fleet for the campus map; locations/statuses are simulated.
        ambulances.emplace_back("AMB-101", "Main Gate", "Available");
        ambulances.emplace_back("AMB-102", "Hostel", "Available");
        ambulances.emplace_back("AMB-103", "Security Office", "Available");
        saveAmbulances();
    }
}

void AmbulanceDispatch::saveAmbulances() const {
    ofstream file(ambulanceFile, ios::trunc);
    for (const Ambulance& ambulance : ambulances) {
        file << ambulance.getId() << '|' << ambulance.getLocation() << '|'
             << ambulance.getStatus() << '|' << ambulance.getAssignedIncidentId() << '\n';
    }
}

void AmbulanceDispatch::appendLog(const string& message) const {
    ofstream file(logFile, ios::app);
    if (!file) {
        cerr << "Warning: could not write to " << logFile << ".\n";
        return;
    }
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    if (localTime) file << put_time(localTime, "%Y-%m-%d %H:%M:%S") << " | ";
    file << message << '\n';
}

Incident* AmbulanceDispatch::findIncident(int id) {
    for (Incident& incident : incidents) if (incident.id == id) return &incident;
    return nullptr;
}

int AmbulanceDispatch::findAmbulanceIndex(const string& ambulanceId) const {
    for (size_t i = 0; i < ambulances.size(); ++i)
        if (ambulances[i].getId() == ambulanceId) return static_cast<int>(i);
    return -1;
}

string AmbulanceDispatch::joinRoute(const vector<string>& route) {
    if (route.empty()) return "(no route)";
    ostringstream output;
    for (size_t i = 0; i < route.size(); ++i) {
        if (i) output << " -> ";
        output << route[i];
    }
    return output.str();
}

void AmbulanceDispatch::displayAmbulances() const {
    cout << "\n========== AMBULANCE FLEET (simulated status) ==========\n";
    if (ambulances.empty()) { cout << "No ambulances registered.\n"; return; }
    // Sorting a copy keeps the stored fleet order unchanged.
    vector<Ambulance> sortedFleet = ambulances;
    sort(sortedFleet.begin(), sortedFleet.end(), [](const Ambulance& a, const Ambulance& b) {
        return a.getId() < b.getId();
    });
    // Vehicle* demonstrates base-class pointers and virtual dispatch.
    for (const Ambulance& ambulance : sortedFleet) {
        const Vehicle* vehicle = &ambulance;
        vehicle->display();
    }
}

void AmbulanceDispatch::dispatchIncident() {
    if (incidents.empty()) { cout << "\nNo incidents are recorded. Report an incident first.\n"; return; }
    int incidentId;
    cout << "\nEnter incident ID to dispatch an ambulance: ";
    if (!(cin >> incidentId)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid ID.\n"; return; }
    Incident* incident = findIncident(incidentId);
    if (!incident) { cout << "Incident not found.\n"; return; }

    string incidentLocation = incident->location;
    if (!graph.hasLocation(incidentLocation)) {
        cout << "Incident location '" << incidentLocation << "' is not a graph node.\n";
        cout << "Choose the mapped location that best represents the incident site:\n";
        vector<string> locations = graph.getLocations();
        for (size_t i = 0; i < locations.size(); ++i)
            cout << (i + 1) << ". " << locations[i] << '\n';
        cout << "Choice: ";
        int selected;
        if (!(cin >> selected)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid choice.\n"; return; }
        if (selected < 1 || selected > static_cast<int>(locations.size())) { cout << "Invalid location choice.\n"; return; }
        incidentLocation = locations[static_cast<size_t>(selected - 1)];
    }

    int selectedIndex = -1;
    int bestDistance = numeric_limits<int>::max();
    vector<string> bestRoute;
    for (size_t i = 0; i < ambulances.size(); ++i) {
        if (ambulances[i].getStatus() != "Available") continue;
        vector<string> route = graph.shortestPath(ambulances[i].getLocation(), incidentLocation);
        if (route.empty()) continue; // Disconnected ambulance cannot be dispatched.
        int distance = graph.shortestDistance(ambulances[i].getLocation(), incidentLocation);
        if (distance < bestDistance) {
            bestDistance = distance;
            selectedIndex = static_cast<int>(i);
            bestRoute = route;
        }
    }

    if (selectedIndex == -1) {
        cout << "No available ambulance can reach this incident. Check fleet status and graph connectivity.\n";
        appendLog("FAILED | incident=" + to_string(incidentId) + " | location=" + incidentLocation + " | reason=no reachable available ambulance");
        return;
    }

    string hospital;
    vector<string> hospitalRoute;
    int hospitalDistance = 0;
    if (!graph.nearestHospitalRoute(incidentLocation, hospital, hospitalRoute, hospitalDistance)) {
        cout << "No hospital is reachable from the incident site. Dispatch cancelled.\n";
        appendLog("FAILED | incident=" + to_string(incidentId) + " | location=" + incidentLocation + " | reason=no reachable hospital");
        return;
    }

    Ambulance& ambulance = ambulances[static_cast<size_t>(selectedIndex)];
    // In this simulation, dispatch moves the ambulance marker to the incident site.
    // Completing the call then moves it to the selected hospital.
    ambulance.setLocation(incidentLocation);
    ambulance.assignTo(incidentId);
    saveAmbulances();

    cout << "\n========== DISPATCH CONFIRMED ==========\n";
    cout << "Incident ID             : " << incident->id << '\n';
    cout << "Incident location used  : " << incidentLocation << '\n';
    cout << "Selected ambulance      : " << ambulance.getId() << '\n';
    cout << "Ambulance-to-incident   : " << joinRoute(bestRoute) << '\n';
    cout << "Distance to incident    : " << bestDistance << " m\n";
    cout << "Nearest reachable hospital: " << hospital << '\n';
    cout << "Incident-to-hospital    : " << joinRoute(hospitalRoute) << '\n';
    cout << "Hospital route distance : " << hospitalDistance << " m\n";
    cout << "Ambulance status         : Busy\n";
    cout << "========================================\n";

    appendLog("SUCCESS | incident=" + to_string(incidentId) + " | ambulance=" + ambulance.getId() +
              " | incident_location=" + incidentLocation + " | ambulance_route=" + joinRoute(bestRoute) +
              " | hospital=" + hospital + " | hospital_route=" + joinRoute(hospitalRoute));
}

void AmbulanceDispatch::updateAmbulanceStatus() {
    string id;
    cout << "\nEnter ambulance ID: ";
    cin >> ws;
    getline(cin, id);
    int index = findAmbulanceIndex(id);
    if (index < 0) { cout << "Ambulance not found.\n"; return; }
    Ambulance& ambulance = ambulances[static_cast<size_t>(index)];
    if (ambulance.getStatus() == "Busy") {
        cout << "This ambulance is assigned to an incident. Complete that dispatch before changing its status.\n";
        return;
    }
    cout << "1. Available\n2. Offline\nChoose new status: ";
    int choice;
    if (!(cin >> choice)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid choice.\n"; return; }
    if (choice == 1) ambulance.setStatus("Available");
    else if (choice == 2) ambulance.setStatus("Offline");
    else { cout << "Invalid status choice.\n"; return; }
    saveAmbulances();
    appendLog("STATUS UPDATE | ambulance=" + id + " | status=" + ambulance.getStatus());
    cout << "Ambulance status updated to " << ambulance.getStatus() << ".\n";
}

void AmbulanceDispatch::completeDispatch() {
    string id;
    cout << "\nEnter ambulance ID whose dispatch is complete: ";
    cin >> ws;
    getline(cin, id);
    int index = findAmbulanceIndex(id);
    if (index < 0) { cout << "Ambulance not found.\n"; return; }
    Ambulance& ambulance = ambulances[static_cast<size_t>(index)];
    if (ambulance.getStatus() != "Busy") { cout << "This ambulance has no active dispatch.\n"; return; }

    string hospital;
    vector<string> route;
    int distance = 0;
    if (!graph.nearestHospitalRoute(ambulance.getLocation(), hospital, route, distance)) {
        cout << "Could not find a hospital route from the ambulance's stored location. Status unchanged.\n";
        return;
    }
    int incidentId = ambulance.getAssignedIncidentId();
    ambulance.clearAssignment(hospital);
    saveAmbulances();
    appendLog("COMPLETED | incident=" + to_string(incidentId) + " | ambulance=" + id + " | new_location=" + hospital);
    cout << "Dispatch marked complete. " << id << " is now Available at " << hospital << ".\n";
}

void AmbulanceDispatch::displayDispatchLog() const {
    ifstream file(logFile);
    if (!file) { cout << "\nNo dispatch log exists yet.\n"; return; }
    cout << "\n================ DISPATCH LOG ================\n";
    string line;
    bool any = false;
    while (getline(file, line)) { cout << line << '\n'; any = true; }
    if (!any) cout << "No dispatch events recorded yet.\n";
    cout << "==============================================\n";
}
