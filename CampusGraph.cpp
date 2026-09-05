#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

struct Incident {
    int id;
    string reporter;
    string location;
    string type;
    int severity;       
    string description;
};

string severityName(int severity) {
    switch (severity) {
        case 1: return "Low";
        case 2: return "Medium";
        case 3: return "High";
        case 4: return "Critical";
        default: return "Unknown";
    }
}

struct IncidentPriority {
    bool operator()(const Incident& a, const Incident& b) const {
        if (a.severity != b.severity)
            return a.severity < b.severity;

        return a.id > b.id;
    }
};

class CampusGraph {
private:
    map<string, vector<pair<string, int>>> adj;

public:
    void addLocation(const string& name) {
        adj[name]; 
    }

    void addPath(const string& from, const string& to, int distance) {
        adj[from].push_back({to, distance});
        adj[to].push_back({from, distance});
    }

    void displayCampus() const {
        cout << "\n========== CAMPUS GRAPH ==========\n";

        for (const auto& node : adj) {
            cout << left << setw(20) << node.first << " -> ";

            for (const auto& edge : node.second) {
                cout << edge.first << "(" << edge.second << "m)  ";
            }
            cout << '\n';
        }

        cout << "==================================\n";
    }

    bool hasLocation(const string& name) const {
        return adj.find(name) != adj.end();
    }
};

void printIncident(const Incident& incident) {
    cout << "\n----------------------------------------\n";
    cout << "Incident ID   : " << incident.id << '\n';
    cout << "Reporter      : " << incident.reporter << '\n';
    cout << "Location      : " << incident.location << '\n';
    cout << "Type          : " << incident.type << '\n';
    cout << "Severity      : " << severityName(incident.severity) << '\n';
    cout << "Description   : " << incident.description << '\n';
    cout << "----------------------------------------\n";
}

void showNormalQueue(queue<Incident> q) {
    if (q.empty()) {
        cout << "\nNo normal emergency requests.\n";
        return;
    }

    cout << "\n========== NORMAL QUEUE ==========\n";
    while (!q.empty()) {
        printIncident(q.front());
        q.pop();
    }
}

void showPriorityQueue(
    priority_queue<Incident, vector<Incident>, IncidentPriority> pq
) {
    if (pq.empty()) {
        cout << "\nNo priority emergency requests.\n";
        return;
    }

    cout << "\n======= PRIORITY QUEUE =======\n";
    while (!pq.empty()) {
        printIncident(pq.top());
        pq.pop();
    }
}

int main() {
    // -------------------------------
    // 1. Campus location database
    // -------------------------------
    CampusGraph campus;

    campus.addLocation("Main Gate");
    campus.addLocation("Academic Block");
    campus.addLocation("Hostel");
    campus.addLocation("Medical Center");
    campus.addLocation("Security Office");
    campus.addLocation("Library");
    campus.addLocation("Cafeteria");

    // Campus paths. Distance is only stored for Phase 1.
    // Dijkstra will use these weights in Phase 2.
    campus.addPath("Main Gate", "Academic Block", 300);
    campus.addPath("Main Gate", "Security Office", 150);
    campus.addPath("Academic Block", "Library", 120);
    campus.addPath("Academic Block", "Cafeteria", 180);
    campus.addPath("Academic Block", "Hostel", 400);
    campus.addPath("Hostel", "Medical Center", 250);
    campus.addPath("Security Office", "Medical Center", 350);

    // -------------------------------
    // 2. Emergency management queues
    // -------------------------------
    queue<Incident> normalQueue;

    priority_queue<Incident, vector<Incident>, IncidentPriority>
        emergencyPriorityQueue;

    int nextIncidentID = 1001;

    int choice;

    do {
        cout << "\n\n========================================\n";
        cout << "       CAMPUS CARE - PHASE 1\n";
        cout << "========================================\n";
        cout << "1. Report Emergency\n";
        cout << "2. Show Normal Emergency Queue\n";
        cout << "3. Show Priority Queue\n";
        cout << "4. Process Next Normal Incident\n";
        cout << "5. Process Highest Priority Incident\n";
        cout << "6. Show Campus Locations & Connections\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(10000, '\n');

        switch (choice) {

        // ---------------------------------
        // REPORT EMERGENCY
        // ---------------------------------
        case 1: {
            Incident incident;
            incident.id = nextIncidentID++;

            cout << "\nEnter reporter name: ";
            getline(cin, incident.reporter);

            cout << "Enter incident location: ";
            getline(cin, incident.location);

            if (!campus.hasLocation(incident.location)) {
                cout << "Warning: location is not in the predefined campus database.\n";
                cout << "For Phase 1, the incident will still be stored.\n";
            }

            cout << "Enter incident type "
                    "(Medical/Fire/Harassment/Accident/Other): ";
            getline(cin, incident.type);

            cout << "Enter severity (1=Low, 2=Medium, 3=High, 4=Critical): ";
            cin >> incident.severity;
            cin.ignore(10000, '\n');

            if (incident.severity < 1 || incident.severity > 4) {
                cout << "Invalid severity. Setting severity to Medium.\n";
                incident.severity = 2;
            }

            cout << "Enter short description: ";
            getline(cin, incident.description);

            // Every incident is stored in a normal FIFO queue.
            normalQueue.push(incident);

            // High and critical incidents also enter the priority queue.
            if (incident.severity >= 3) {
                emergencyPriorityQueue.push(incident);
            }

            cout << "\nEmergency reported successfully!\n";
            cout << "Generated Incident ID: " << incident.id << '\n';
            cout << "Assigned Severity: " << severityName(incident.severity) << '\n';

            break;
        }

        // ---------------------------------
        // DISPLAY NORMAL QUEUE
        // ---------------------------------
        case 2:
            showNormalQueue(normalQueue);
            break;

        // ---------------------------------
        // DISPLAY PRIORITY QUEUE
        // ---------------------------------
        case 3:
            showPriorityQueue(emergencyPriorityQueue);
            break;

        // ---------------------------------
        // PROCESS FIFO INCIDENT
        // ---------------------------------
        case 4:
            if (normalQueue.empty()) {
                cout << "\nNo incident available in normal queue.\n";
            } else {
                cout << "\nProcessing next incident (FIFO order):\n";
                printIncident(normalQueue.front());
                normalQueue.pop();
            }
            break;

        // ---------------------------------
        // PROCESS HIGHEST PRIORITY INCIDENT
        // ---------------------------------
        case 5:
            if (emergencyPriorityQueue.empty()) {
                cout << "\nNo high-priority incident available.\n";
            } else {
                cout << "\nProcessing highest-priority incident:\n";
                printIncident(emergencyPriorityQueue.top());
                emergencyPriorityQueue.pop();
            }
            break;

        // ---------------------------------
        // DISPLAY CAMPUS GRAPH
        // ---------------------------------
        case 6:
            campus.displayCampus();
            cout << "\nNote: shortest-route calculation using Dijkstra's\n"
                    "algorithm will be added in Phase 2.\n";
            break;

        case 0:
            cout << "\nExiting Campus Care. Thank you.\n";
            break;

        default:
            cout << "\nInvalid menu choice.\n";
        }

    } while (choice != 0);

    return 0;
}