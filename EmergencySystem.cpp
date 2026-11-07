
#include "EmergencySystem.h"
#include <iostream>
#include <fstream>

using namespace std;

EmergencySystem::EmergencySystem() {
  loadProcessedIncidents();
}

EmergencySystem::EmergencySystem(
    const vector<Incident>& existingIncidents
) {
    loadProcessedIncidents();
    for (const Incident& incident : existingIncidents) {
        addIncident(incident);
    }
}

void EmergencySystem::loadProcessedIncidents() {

    ifstream file("processed_incidents.txt");

    if (!file) {
        return;
    }

    int id;

    while (file >> id) {
        processedIncidents.insert(id);
    }

}

void EmergencySystem::saveProcessedIncidents() const {

    ofstream file("processed_incidents.txt");

    if (!file) {
        cout << "Error saving processed incidents.\n";
        return;
    }

    for (int id : processedIncidents) {
        file << id << '\n';
    }

}


// Converts severity number to text
string severityName(int severity) {
    switch (severity) {
        case 1:
            return "Low";

        case 2:
            return "Medium";

        case 3:
            return "High";

        case 4:
            return "Critical";

        default:
            return "Unknown";
    }
}

// Displays one incident
void printIncident(const Incident& incident) {
    cout << "\n----------------------------------------\n";

    cout << "Incident ID   : " << incident.id << '\n';
    cout << "Reporter      : " << incident.reporter << '\n';
    cout << "Location      : " << incident.location << '\n';
    cout << "Type          : " << incident.type << '\n';

    cout << "Severity      : "
         << severityName(incident.severity) << '\n';

    cout << "Description   : " << incident.description << '\n';

    cout << "----------------------------------------\n";
}

// Adds an incident to the emergency system
void EmergencySystem::addIncident(const Incident& incident) {

    // Every incident enters the normal FIFO queue
    normalQueue.push(incident);

    // High and Critical incidents also enter
    // the severity-based priority queue
    if (incident.severity >= 3) {
        emergencyPriorityQueue.push(incident);
    }
}

// NEW: Removes an incident from both queues
void EmergencySystem::removeIncident(int incidentId) {

    // Rebuild the normal queue without the deleted incident
    queue<Incident> updatedNormalQueue;

    while (!normalQueue.empty()) {

        Incident incident = normalQueue.front();
        normalQueue.pop();

        if (incident.id != incidentId) {
            updatedNormalQueue.push(incident);
        }
    }

    normalQueue = updatedNormalQueue;


    // Rebuild the priority queue without the deleted incident
    priority_queue<
        Incident,
        vector<Incident>,
        IncidentPriority
    > updatedPriorityQueue;

    while (!emergencyPriorityQueue.empty()) {

        Incident incident = emergencyPriorityQueue.top();
        emergencyPriorityQueue.pop();

        if (incident.id != incidentId) {
            updatedPriorityQueue.push(incident);
        }
    }

    emergencyPriorityQueue = updatedPriorityQueue;


    // Remove the incident from the processed set too
    processedIncidents.erase(incidentId);
    saveProcessedIncidents();

    cout << "\nIncident #" << incidentId
         << " removed from emergency queues.\n";
}

// Displays normal queue without modifying the actual queue
void EmergencySystem::showNormalQueue() {

    if (normalQueue.empty()) {
        cout << "\nNo normal emergency requests.\n";
        return;
    }

    cout << "\n========== NORMAL QUEUE ==========\n";

    queue<Incident> temp = normalQueue;

    while (!temp.empty()) {

        Incident incident = temp.front();
        temp.pop();

        if (processedIncidents.find(incident.id)
            == processedIncidents.end()) {

            printIncident(incident);
        }
    }
}

// Displays priority queue without modifying the actual queue
void EmergencySystem::showPriorityQueue() {

    if (emergencyPriorityQueue.empty()) {
        cout << "\nNo priority emergency requests.\n";
        return;
    }

    cout << "\n======= PRIORITY QUEUE =======\n";

    auto temp = emergencyPriorityQueue;

    while (!temp.empty()) {

        Incident incident = temp.top();
        temp.pop();

        if (processedIncidents.find(incident.id)
            == processedIncidents.end()) {

            printIncident(incident);
        }
    }
}

// Processes the next normal incident using FIFO
void EmergencySystem::processNextNormal() {

    while (!normalQueue.empty()) {

        Incident incident = normalQueue.front();
        normalQueue.pop();

        if (processedIncidents.find(incident.id)
            != processedIncidents.end()) {

            continue;
        }

        cout << "\nProcessing next incident (FIFO order):\n";

        printIncident(incident);

        processedIncidents.insert(incident.id);
        saveProcessedIncidents();

        return;
    }

    cout << "\nNo incident available in normal queue.\n";
}

// Processes the highest-severity incident
void EmergencySystem::processHighestPriority() {

    while (!emergencyPriorityQueue.empty()) {

        Incident incident = emergencyPriorityQueue.top();
        emergencyPriorityQueue.pop();

        if (processedIncidents.find(incident.id)
            != processedIncidents.end()) {

            continue;
        }

        cout << "\nProcessing highest-priority incident:\n";

        printIncident(incident);

        processedIncidents.insert(incident.id);
        saveProcessedIncidents();

        return;
    }

    cout << "\nNo high-priority incident available.\n";
}
