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
    bool dataChanged = false;

    if (file) {
        string line;

        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream row(line);
            string id, location, status, incidentText;

            if (!(getline(row, id, '|') &&
                  getline(row, location, '|') &&
                  getline(row, status, '|') &&
                  getline(row, incidentText))) {
                continue;
            }

            try {
                int assigned = stoi(incidentText);

                // Validate the ambulance data.
                if (id.empty() ||
                    !graph.hasLocation(location) ||
                    (status != "Available" &&
                     status != "Busy" &&
                     status != "Offline")) {
                    continue;
                }

                // Prevent duplicate ambulance IDs.
                if (findAmbulanceIndex(id) != -1) {
                    continue;
                }

                // A Busy ambulance must have a valid incident.
                if (status == "Busy") {
                    if (assigned == -1 || findIncident(assigned) == nullptr) {
                        status = "Available";
                        assigned = -1;
                        dataChanged = true;
                    }
                } else {
                    // Available/Offline ambulances should not
                    // retain an incident assignment.
                    if (assigned != -1) {
                        assigned = -1;
                        dataChanged = true;
                    }
                }

                ambulances.emplace_back(id, location, status, assigned);

            } catch (...) {
                // Ignore malformed data rows.
            }
        }
    }

    // Create a demo fleet if no valid ambulance data was loaded.
    if (ambulances.empty()) {
        ambulances.emplace_back("AMB-101", "Main Gate", "Available");
        ambulances.emplace_back("AMB-102", "Hostel", "Available");
        ambulances.emplace_back("AMB-103", "Security Office", "Available");

        dataChanged = true;
    }

    if (dataChanged) {                                                                      
        saveAmbulances();
    }
}


void AmbulanceDispatch::saveAmbulances() const {
    ofstream file(ambulanceFile, ios::trunc);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << ambulanceFile
             << " for writing.\n";
        return;
    }

    for (const Ambulance& ambulance : ambulances) {
        file << ambulance.getId() << '|'
             << ambulance.getLocation() << '|'
             << ambulance.getStatus() << '|'
             << ambulance.getAssignedIncidentId() << '\n';

        if (!file) {
            cerr << "Error: Failed while writing ambulance data.\n";
            return;
        }
    }

    file.close();

    if (file.fail()) {
        cerr << "Error: Failed to close " << ambulanceFile << ".\n";
    }
}



void AmbulanceDispatch::appendLog(const string& message) const {
    ofstream file(logFile, ios::app);

    if (!file.is_open()) {
        cerr << "Warning: Could not open " << logFile
             << " for writing.\n";
        return;
    }

    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    if (localTime != nullptr) {
        file << put_time(localTime, "%Y-%m-%d %H:%M:%S")
             << " | ";
    } else {
        file << "Unknown time | ";
    }

    file << message << '\n';

    if (!file) {
        cerr << "Warning: Failed to write dispatch log entry.\n";
    }
}
Incident* AmbulanceDispatch::findIncident(int id) {
    for (Incident& incident : incidents) {
        if (incident.id == id) {
            return &incident;
        }
    }

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
    if (ambulances.empty()) { cout << "No ambulances registered.\n"; return ; }
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

int AmbulanceDispatch::dispatchIncident() {

    if (incidents.empty()) { cout << "\nNo incidents are recorded. Report an incident first.\n"; return -1; }
    int incidentId;

    cout << "\nEnter incident ID to dispatch an ambulance: ";

    if (!(cin >> incidentId)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid ID.\n"; return -1; }
    Incident* incident = findIncident(incidentId);
    if (!incident) 
    {
         cout << "Incident not found.\n";
          return -1; }
// Check whether this incident already has an ambulance assigned.
for (const Ambulance& ambulance : ambulances) {
    if (ambulance.getAssignedIncidentId() == incidentId &&
        ambulance.getStatus() == "Busy") {

        cout << "This incident already has an ambulance assigned: "
             << ambulance.getId() << '\n';

        cout << "Complete the existing dispatch before assigning another ambulance.\n";
        return -1;
    }
}


    string incidentLocation = incident->location;
    if (!graph.hasLocation(incidentLocation)) {
        cout << "Incident location '" << incidentLocation << "' is not a graph node.\n";
        cout << "Choose the mapped location that best represents the incident site:\n";
        vector<string> locations = graph.getLocations();
        for (size_t i = 0; i < locations.size(); ++i)
            cout << (i + 1) << ". " << locations[i] << '\n';
        cout << "Choice: ";
        int selected;
        if (!(cin >> selected)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid choice.\n"; return -1; }
        if (selected < 1 || selected > static_cast<int>(locations.size())) { cout << "Invalid location choice.\n"; return -1; }
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
        return -1;
    }

    string hospital;
    vector<string> hospitalRoute;
    int hospitalDistance = 0;
    if (!graph.nearestHospitalRoute(incidentLocation, hospital, hospitalRoute, hospitalDistance)) {
        cout << "No hospital is reachable from the incident site. Dispatch cancelled.\n";
        appendLog("FAILED | incident=" + to_string(incidentId) + " | location=" + incidentLocation + " | reason=no reachable hospital");
        return incidentId;
        return -1;
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
              return incidentId;
}


void AmbulanceDispatch::updateAmbulanceStatus() {
    string id;

    cout << "\nEnter ambulance ID: ";
    getline(cin >> ws, id);

    if (id.empty()) {
        cout << "Ambulance ID cannot be empty.\n";
        return;
    }

    int index = findAmbulanceIndex(id);

    if (index < 0) {
        cout << "Ambulance not found.\n";
        return;
    }

    Ambulance& ambulance = ambulances[static_cast<size_t>(index)];

    if (ambulance.getStatus() == "Busy") {
        cout << "This ambulance is assigned to an incident. "
             << "Complete that dispatch before changing its status.\n";
        return;
    }

    cout << "\n1. Available\n";
    cout << "2. Offline\n";
    cout << "Choose new status: ";

    int choice;

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Please enter a number.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {
        ambulance.setStatus("Available");
    }
    else if (choice == 2) {
        ambulance.setStatus("Offline");
    }
    else {
        cout << "Invalid status choice. Choose 1 or 2.\n";
        return;
    }

    saveAmbulances();

    appendLog("STATUS UPDATE | ambulance=" + id +
              " | status=" + ambulance.getStatus());

    cout << "Ambulance status updated to "
         << ambulance.getStatus() << ".\n";
}


int AmbulanceDispatch::completeDispatch() {
    string id;
    cout << "\nEnter ambulance ID whose dispatch is complete: ";
    cin >> ws;
    getline(cin, id);
    int index = findAmbulanceIndex(id);
    if (index < 0) { cout << "Ambulance not found.\n"; return -1; }
    Ambulance& ambulance = ambulances[static_cast<size_t>(index)];
    if (ambulance.getStatus() != "Busy") { cout << "This ambulance has no active dispatch.\n"; return-1; }

    
int incidentId = ambulance.getAssignedIncidentId();

// Check whether the assigned incident still exists.
if (incidentId == -1 || findIncident(incidentId) == nullptr) {
    cout << "The assigned incident no longer exists.\n";
    cout << "Resetting ambulance status to Available.\n";

    ambulance.clearAssignment(ambulance.getLocation());
    saveAmbulances();

    appendLog("RECOVERY | ambulance=" + id +
              " | reason=assigned incident no longer exists");

    return -1;
}

    string hospital;
    vector<string> route;   
    int distance = 0;
    if (!graph.nearestHospitalRoute(ambulance.getLocation(), hospital, route, distance)) {
        cout << "Could not find a hospital route from the ambulance's stored location. Status unchanged.\n";
        return -1                                                                                                                                                                   ;
    }
  
    ambulance.clearAssignment(hospital);
    saveAmbulances();
    appendLog("COMPLETED | incident=" + to_string(incidentId) + " | ambulance=" + id + " | new_location=" + hospital);
    cout << "Dispatch marked complete. " << id << " is now Available at " << hospital << ".\n";
    return incidentId;
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
int AmbulanceDispatch::getTotalAmbulances() const {
    return static_cast<int>(ambulances.size());
}

int AmbulanceDispatch::getAvailableAmbulances() const {
    int count = 0;

    for (const Ambulance& ambulance : ambulances) {
        if (ambulance.getStatus() == "Available") {
            count++;
        }
    }

    return count;
}