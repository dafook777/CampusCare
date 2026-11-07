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

   
Incident IncidentManager::addIncident() {

    Incident incident;

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


