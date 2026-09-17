#include <iostream>
#include "ContactManager.h"
#include "incident_manager.h"
#include "EmergencySystem.h"
#include "CampusGraph.h"

using namespace std;

int main() {

    ContactManager contactManager;
IncidentManager incidentManager;

EmergencySystem emergencySystem(
    incidentManager.getIncidents()
);

CampusGraph campus;

    int choice;

     campus.addLocation("Main Gate");
    campus.addLocation("Academic Block");
    campus.addLocation("Hostel");
    campus.addLocation("Medical Center");
    campus.addLocation("Security Office");
    campus.addLocation("Library");
    campus.addLocation("Cafeteria");


  
    campus.addPath("Main Gate", "Academic Block", 300);
    campus.addPath("Main Gate", "Security Office", 150);
    campus.addPath("Academic Block", "Library", 120);
    campus.addPath("Academic Block", "Cafeteria", 180);
    campus.addPath("Academic Block", "Hostel", 400);
    campus.addPath("Hostel", "Medical Center", 250);
    campus.addPath("Security Office", "Medical Center", 350);

    do {

        cout << "\n=====================================\n";
        cout << "          CAMPUSCARE\n";
        cout << "   Student Safety System\n";
        cout << "=====================================\n";

        cout << "1. Add Emergency Contact\n";
        cout << "2. Display Emergency Contacts\n";
        cout << "3. Search Emergency Contact\n";
        cout << "4. Delete Emergency Contact\n";
        cout << "5. Add Incident\n";
        cout << "6. Display Incidents\n";
        cout << "7. Search Incident\n";
        cout << "8. Delete Incident\n";

         cout << "\n--- EMERGENCY QUEUES ---\n";
        cout << "9. Show Normal Emergency Queue\n";
        cout << "10. Show Priority Queue\n";
        cout << "11. Process Next Normal Incident\n";
        cout << "12. Process Highest Priority Incident\n";

        cout << "\n--- CAMPUS ---\n";
        cout << "13. Show Campus Locations & Connections\n";

        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                contactManager.addContact();
                break;

            case 2:
                contactManager.displayContacts();
                break;

            case 3:
                contactManager.searchContact();
                break;

            case 4:
                contactManager.deleteContact();
                break;

            case 5: {

                 Incident incident = incidentManager.addIncident();

             
                 emergencySystem.addIncident(incident);

                  break;
            }

            case 6:
                incidentManager.displayIncidents();
                break;

            case 7:
                incidentManager.searchIncident();
                break;

            case 8:
                incidentManager.deleteIncident();
                break;

            case 9:
                emergencySystem.showNormalQueue();
                break;

            case 10:
                emergencySystem.showPriorityQueue();
                break;

            case 11:
                emergencySystem.processNextNormal();
                break;

            case 12:
                emergencySystem.processHighestPriority();
                break;


        

            case 13:
                campus.displayCampus();
                break;

            case 0:
                contactManager.saveData();
                cout << "\nExiting CampusCare...\n";
                break;

            default:
                cout << "\nInvalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}