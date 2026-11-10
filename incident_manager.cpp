#include "incident_manager.h"
#include "FileManager.h"
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

//helper fn
string readNonEmptyLine(const string& prompt) {

    string value;

    while (true) {

        cout << prompt;
        getline(cin >> ws, value);

        if (value.find_first_not_of(" \t") != string::npos) {
            return value;
        }

        cout << "Input cannot be empty. Please try again.\n";
    }
}


IncidentManager::IncidentManager()
 {
    incidents = FileManager::loadIncidents();

    // Populate the hash table from saved incidents
    for (const Incident& incident : incidents) {
        incidentHashTable[incident.id] = incident;
}

 }

   
Incident IncidentManager::addIncident(const string& ownerId) {
    Incident incident;
    incident.ownerId = ownerId;
    incident.status= "pending";

    cout << "\n========== REPORT EMERGENCY ==========\n";

    // Automatically generate ID
    int maxID = 1000;

    for (const Incident& existing : incidents) {
        if (existing.id > maxID)
            maxID = existing.id;
    }

    incident.id = maxID + 1;

    cout << "Generated Incident ID: "
         << incident.id << '\n';

    // Validate text fields
    incident.reporter =
        readNonEmptyLine("Enter reporter name: ");

    incident.location =
        readNonEmptyLine("Enter incident location: ");

    incident.type =
        readNonEmptyLine("Enter incident type: ");

    // Validate severity
    while (true) {

        cout << "Enter severity "
             << "(1=Low, 2=Medium, 3=High, 4=Critical): ";

        if (cin >> incident.severity) {

            if (incident.severity >= 1 &&
                incident.severity <= 4) {

                break;
            }

            cout << "Severity must be between 1 and 4.\n";
        }
        else {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input. Enter a number from 1 to 4.\n";
        }
    }

    incident.description =
        readNonEmptyLine("Enter description: ");

    // Add incident to vector
    incidents.push_back(incident);

    // Add incident to hash table
    incidentHashTable[incident.id] = incident;

    // Save incidents to text file
    FileManager::saveIncidents(incidents);

    cout << "\nEmergency added successfully!\n";

    return incident;
}


void IncidentManager::displayIncidents() {

    if (incidents.empty()) {
        cout << "\nNo incidents recorded.\n";
        return;
    }

    cout << "\n========== INCIDENTS ==========\n";

    for (const Incident& incident : incidents) {

        cout << "\nIncident ID : " << incident.id;
        cout << "\nReporter    : " << incident.reporter;
        cout << "\nLocation    : " << incident.location;
        cout << "\nType        : " << incident.type;
        cout << "\nSeverity    : " << incident.severity;
        cout << "\nDescription : " << incident.description;
        cout << "\n-------------------------------\n";
    }
}


void IncidentManager::displayMyReports(const string& ownerId) {

    bool found = false;

    cout << "\n========== MY INCIDENT REPORTS ==========\n";

    for (const Incident& incident : incidents) {

        if (incident.ownerId == ownerId) {

            found = true;

            cout << "\nIncident ID : " << incident.id;
            cout << "\nReporter    : " << incident.reporter;
            cout << "\nLocation    : " << incident.location;
            cout << "\nType        : " << incident.type;
            cout << "\nSeverity    : " << incident.severity;
            cout << "\nDescription : " << incident.description;
            cout << "\nStatus      : " << incident.status;
            cout << "\n-----------------------------------------\n";
        }
    }

    if (!found) {
        cout << "\nYou haven't submitted any incident reports yet.\n";
    }
}


bool IncidentManager::updateIncidentStatus(Incident& updatedIncident) {

    int id;

    cout << "\nEnter incident ID to update: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nInvalid incident ID.\n";
        return false;
    }

    // Find the incident using the hash table
    auto it = incidentHashTable.find(id);

    if (it == incidentHashTable.end()) {
        cout << "\nIncident not found.\n";
        return false;
    }

    cout << "\nCurrent status: " << it->second.status << '\n';

    cout << "\nChoose new status:\n";
    cout << "1. Pending\n";
    cout << "2. Under Review\n";
    cout << "3. In Progress\n";
    cout << "4. Resolved\n";
    cout << "Enter choice: ";

    int statusChoice;

    if (!(cin >> statusChoice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nInvalid choice.\n";
        return false;
    }

    string newStatus;

    switch (statusChoice) {
        case 1:
            newStatus = "Pending";
            break;

        case 2:
            newStatus = "Under Review";
            break;

        case 3:
            newStatus = "In Progress";
            break;

        case 4:
            newStatus = "Resolved";
            break;

        default:
            cout << "\nInvalid status choice.\n";
            return false;
    }

    // Update the incident in the main vector
    for (Incident& incident : incidents) {

        if (incident.id == id) {

            incident.status = newStatus;

            // Update the hash table copy
            incidentHashTable[id] = incident;

            // Save updated incidents
            FileManager::saveIncidents(incidents);

            // Return the updated incident to main.cpp
            updatedIncident = incident;

            cout << "\nIncident status updated successfully!\n";
            cout << "New status: " << newStatus << '\n';

            return true;
        }
    }

    cout << "\nIncident not found.\n";
    return false;
}

   

void IncidentManager::searchIncident() {

    int id;

    cout << "\nEnter incident ID to search: ";

    if (!(cin >> id)) {

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nInvalid incident ID.\n";
        return;
    }

    // Search using hash table
    auto it = incidentHashTable.find(id);

    if (it != incidentHashTable.end()) {

        const Incident& incident = it->second;

        cout << "\nIncident Found!\n";
        cout << "ID          : " << incident.id << '\n';
        cout << "Reporter    : " << incident.reporter << '\n';
        cout << "Location    : " << incident.location << '\n';
        cout << "Type        : " << incident.type << '\n';
        cout << "Severity    : " << incident.severity << '\n';
        cout << "Description : " << incident.description << '\n';

    }
    else {

        cout << "\nIncident not found.\n";
    }
}


int IncidentManager::deleteIncident() {

    int id;

    cout << "\nEnter incident ID to delete: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nInvalid incident ID.\n";
        return -1;
    }

    for (auto it = incidents.begin(); it != incidents.end(); ++it) {

        if (it->id == id) {

            // Remove from hash table
            incidentHashTable.erase(id);

            // Save the deleted ID before erasing
            int deletedId = it->id;

            // Remove from vector
            incidents.erase(it);

            // Update incidents.txt
            FileManager::saveIncidents(incidents);

            cout << "\nIncident deleted successfully.\n";

            // Return the deleted incident ID
            return deletedId;
        }
    }

    cout << "\nIncident not found.\n";

    return -1;
}
  
void IncidentManager::sortIncidentsBySeverity() {

    vector<Incident> sortedIncidents = incidents;

    // Higher severity number comes first
    sort(sortedIncidents.begin(), sortedIncidents.end(),
        [](const Incident& a, const Incident& b) {
            return a.severity > b.severity;
        });

    cout << "\n===== INCIDENTS SORTED BY SEVERITY =====\n";

    for (const Incident& incident : sortedIncidents) {
        cout << "\nIncident ID : " << incident.id
             << "\nReporter    : " << incident.reporter
             << "\nLocation    : " << incident.location
             << "\nType        : " << incident.type
             << "\nSeverity    : " << incident.severity
             << "\nDescription : " << incident.description
             << "\n----------------------------------------\n";
    }
}


void IncidentManager::sortIncidentsById() {

    vector<Incident> sortedIncidents = incidents;

    // Highest incident ID comes first
    sort(sortedIncidents.begin(), sortedIncidents.end(),
        [](const Incident& a, const Incident& b) {
            return a.id > b.id;
        });

    cout << "\n===== INCIDENTS SORTED BY ID =====\n";

    for (const Incident& incident : sortedIncidents) {
        cout << "\nIncident ID : " << incident.id
             << "\nReporter    : " << incident.reporter
             << "\nLocation    : " << incident.location
             << "\nType        : " << incident.type
             << "\nSeverity    : " << incident.severity
             << "\nDescription : " << incident.description
             << "\n----------------------------------------\n";
    }
}


void IncidentManager::sortIncidentsByLocation() {

    vector<Incident> sortedIncidents = incidents;

    // Alphabetical order of location
    sort(sortedIncidents.begin(), sortedIncidents.end(),
        [](const Incident& a, const Incident& b) {
            return a.location < b.location;
        });

    cout << "\n===== INCIDENTS SORTED BY LOCATION =====\n";

    for (const Incident& incident : sortedIncidents) {
        cout << "\nIncident ID : " << incident.id
             << "\nReporter    : " << incident.reporter
             << "\nLocation    : " << incident.location
             << "\nType        : " << incident.type
             << "\nSeverity    : " << incident.severity
             << "\nDescription : " << incident.description
             << "\n----------------------------------------\n";
    }
}
    vector<Incident>& IncidentManager::getIncidents() {
    return incidents;
}


int IncidentManager::getTotalIncidents() const {
    return static_cast<int>(incidents.size());
}

int IncidentManager::getPendingIncidents() const {
    int count = 0;

    for (const Incident& incident : incidents) {
        if (incident.status == "pending" ||
            incident.status == "Pending" ||
            incident.status == "Under Review" ||
            incident.status == "In Progress") {
            count++;
        }
    }

    return count;
}

int IncidentManager::getResolvedIncidents() const {
    int count = 0;

    for (const Incident& incident : incidents) {
        if (incident.status == "Resolved" ||
            incident.status == "resolved") {
            count++;
        }
    }

    return count;
}

vector<int> IncidentManager::getSeverityCounts() const {
    vector<int> counts(4, 0);

    for (const Incident& incident : incidents) {
        if (incident.severity >= 1 && incident.severity <= 4) {
            counts[incident.severity - 1]++;
        }
    }

    return counts;
}