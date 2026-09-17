#include "incident_manager.h"
#include "FileManager.h"
#include <iostream>
#include <string>

using namespace std;

IncidentManager::IncidentManager()
 {
    incidents = FileManager::loadIncidents();
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

    cout << "Enter reporter name: ";
    getline(cin >> ws, incident.reporter);

    cout << "Enter incident location: ";
    getline(cin, incident.location);

    cout << "Enter incident type: ";
    getline(cin, incident.type);

    cout << "Enter severity "
            "(1=Low, 2=Medium, 3=High, 4=Critical): ";

    cin >> incident.severity;

    if (incident.severity < 1 || incident.severity > 4) {

        cout << "Invalid severity. Setting to Medium.\n";

        incident.severity = 2;
    }

    cout << "Enter description: ";
    getline(cin >> ws, incident.description);

    incidents.push_back(incident);

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
    cin >> id;

    for (const Incident& incident : incidents) {

        if (incident.id == id) {

            cout << "\nIncident Found!\n";
            cout << "ID          : " << incident.id << '\n';
            cout << "Reporter    : " << incident.reporter << '\n';
            cout << "Location    : " << incident.location << '\n';
            cout << "Type        : " << incident.type << '\n';
            cout << "Severity    : " << incident.severity << '\n';
            cout << "Description : " << incident.description << '\n';

            return;
        }
    }

    cout << "\nIncident not found.\n";
}

void IncidentManager::deleteIncident() {

    int id;

    cout << "\nEnter incident ID to delete: ";
    cin >> id;

    for (auto it = incidents.begin(); it != incidents.end(); ++it) {

        if (it->id == id) {

            incidents.erase(it);
            FileManager::saveIncidents(incidents);

            cout << "\nIncident deleted successfully.\n";
            return;
        }
    }

    cout << "\nIncident not found.\n";
}

vector<Incident>& IncidentManager::getIncidents() {
    return incidents;
}