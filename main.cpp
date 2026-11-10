#include <iostream>
#include "ContactManager.h"
#include "incident_manager.h"
#include "EmergencySystem.h"
#include "CampusGraph.h"
#include "AmbulanceDispatch.h"
#include "ConcernManager.h"
#include "MessageManager.h"
#include "NotificationManager.h"
#include "StudentManager.h"
#include <string>
#include<vector>
#include<limits>
#include<fstream>
#include<sstream>
#include <iomanip>


using namespace std;

void displayAdminDashboard(
    const IncidentManager& incidentManager,
    const AmbulanceDispatch& ambulanceDispatch
) {
    int totalIncidents = incidentManager.getTotalIncidents();
    int unresolvedIncidents = incidentManager.getPendingIncidents();
    int resolvedIncidents = incidentManager.getResolvedIncidents();

    int totalAmbulances = ambulanceDispatch.getTotalAmbulances();
    int availableAmbulances = ambulanceDispatch.getAvailableAmbulances();

    vector<int> severityCounts = incidentManager.getSeverityCounts();

    cout << "\n";
    cout << "============================================\n";
    cout << "          CAMPUSCARE ADMIN DASHBOARD\n";
    cout << "============================================\n";

    cout << "\n---------- INCIDENT OVERVIEW ----------\n";
    cout << "Total Incidents       : " << totalIncidents << '\n';
    cout << "Unresolved Incidents  : " << unresolvedIncidents << '\n';
    cout << "Resolved Incidents    : " << resolvedIncidents << '\n';

    cout << "\n---------- AMBULANCE OVERVIEW ----------\n";
    cout << "Total Ambulances      : " << totalAmbulances << '\n';
    cout << "Available Ambulances  : " << availableAmbulances << '\n';
    cout << "Busy / Offline        : "
         << totalAmbulances - availableAmbulances << '\n';

    cout << "\n---------- INCIDENTS BY SEVERITY ----------\n";

    const string severityNames[] = {
        "Low", "Medium", "High", "Critical"
    };

    for (int i = 0; i < 4; i++) {
        cout << severityNames[i] << " Incidents : "
             << severityCounts[i];

        if (totalIncidents > 0) {
            double percentage =
                (static_cast<double>(severityCounts[i]) / totalIncidents) * 100;

            cout << " (" << fixed << setprecision(1)
                 << percentage << "%)";
        }

        cout << '\n';
    }

    cout << "\n============================================\n";
}



bool updateFacultyAccountStatus(const string& facultyId, bool activate) {

    ifstream inputFile("users.txt");

    if (!inputFile.is_open()) {
        cout << "\nError: Could not open users.txt\n";
        return false;
    }

    ofstream tempFile("users_temp.txt");

    if (!tempFile.is_open()) {
        cout << "\nError: Could not create temporary file.\n";
        return false;
    }

    string line;
    bool found = false;
    bool changed = false;

    while (getline(inputFile, line)) {

        istringstream row(line);
        string id, role;
        row >> id >> role;

        if (id == facultyId && role == "faculty") {

            int currentStatus = 0;
            row >> currentStatus;

            // Always preserve the faculty record.
            tempFile << id << " " << role << " "
                     << (activate ? 1 : 0) << '\n';

            found = true;

            if (currentStatus == (activate ? 1 : 0)) {
                cout << "\nFaculty account is already "
                     << (activate ? "active.\n" : "inactive.\n");
            }
            else {
                changed = true;
            }

        }
        else {
            tempFile << line << '\n';
        }
    }

    inputFile.close();
    tempFile.close();

    if (!found) {
        remove("users_temp.txt");
        cout << "\nFaculty account not found.\n";
        return false;
    }

    if (!changed) {
        remove("users_temp.txt");
        return true;
    }

    if (remove("users.txt") != 0) {
        remove("users_temp.txt");
        cout << "\nError: Could not replace users.txt.\n";
        return false;
    }

    if (rename("users_temp.txt", "users.txt") != 0) {
        cout << "\nError: Could not update users.txt.\n";
        return false;
    }

    cout << "\nFaculty account "
         << (activate ? "reactivated" : "deactivated")
         << " successfully!\n";

    return true;
}


bool registerUser(const string& userId, const string& role) {

    if (userId.empty()) {
        cout << "\nUser ID cannot be empty.\n";
        return false;
    }

    if (role != "student" && role != "faculty") {
        cout << "\nInvalid user role.\n";
        return false;
    }

    ifstream inputFile("users.txt");

    if (!inputFile.is_open()) {
        cout << "\nError: Could not open users.txt.\n";
        return false;
    }

    string registeredId, registeredRole;
    bool alreadyExists = false;

    while (inputFile >> registeredId >> registeredRole) {

        if (registeredId == userId) {
            alreadyExists = true;
            break;
        }

        string remainingData;
        getline(inputFile, remainingData);
    }

    inputFile.close();

    if (alreadyExists) {
        cout << "\nThis user ID is already registered.\n";
        return false;
    }

    ofstream outputFile("users.txt", ios::app);

    if (!outputFile.is_open()) {
        cout << "\nError: Could not update users.txt.\n";
        return false;
    }

    if (role == "student") {
        outputFile << userId << " student\n";
    }
    else {
        outputFile << userId << " faculty 1\n";
    }

    outputFile.close();

    if (!outputFile) {
        cout << "\nError while saving the account.\n";
        return false;
    }

    cout << "\n================================\n";
    cout << "     USER REGISTERED!\n";
    cout << "================================\n";
    cout << "User ID : " << userId << '\n';
    cout << "Role    : " << role << '\n';
    cout << "Status  : Active\n";

    if (role == "student") {
        cout << "\nStudent profile will be created on first login.\n";
    }

    return true;
}


int main() {

    ContactManager contactManager;
    IncidentManager incidentManager;
    ConcernManager concernManager;
    MessageManager messageManager;
    NotificationManager notificationManager;
    StudentManager studentManager;

EmergencySystem emergencySystem(
    incidentManager.getIncidents()
);


CampusGraph campus;

int choice;

// ================= REAL GEHU CAMPUS LOCATIONS =================

campus.addLocation("New Audi");
campus.addLocation("Fashion dept");
campus.addLocation("CSE dept");
campus.addLocation("GEHU ground");
campus.addLocation("Open Audi");
campus.addLocation("Basement");
campus.addLocation("Cafeteria");
campus.addLocation("Gate 1");
campus.addLocation("Tuck shop");
campus.addLocation("Mechanical lab");
campus.addLocation("Main gate");
campus.addLocation("Girls hostel");
campus.addLocation("Civil lab");
campus.addLocation("Tyagi Clinic");
campus.addLocation("Hospital");

// ================= CAMPUS PATHS (BIDIRECTIONAL) =================

// Floors
campus.addPath("New Audi", "Fashion dept", 90);
campus.addPath("Fashion dept", "CSE dept", 90);
campus.addPath("CSE dept", "Open Audi", 250);

// Main campus connections
campus.addPath("GEHU ground", "Open Audi", 32.5);
campus.addPath("GEHU ground", "Basement", 225);
campus.addPath("Open Audi", "Basement", 125);

campus.addPath("Cafeteria", "Open Audi", 200);
campus.addPath("Cafeteria", "Gate 1", 80);

campus.addPath("Gate 1", "Open Audi", 65);
campus.addPath("Gate 1", "Tuck shop", 17.5);

campus.addPath("Basement", "Tuck shop", 45);
campus.addPath("Basement", "Mechanical lab", 175);

campus.addPath("Mechanical lab", "Civil lab", 65);

campus.addPath("Main gate", "Gate 1", 350);
campus.addPath("Main gate", "Girls hostel", 85);
campus.addPath("Tuck shop", "Girls hostel", 95);
campus.addPath("Gate 1", "Tyagi Clinic", 400);
campus.addPath("Main gate", "Hospital", 1300);

    
AmbulanceDispatch ambulanceDispatch(campus, incidentManager.getIncidents());


    // ================= LOGIN SYSTEM =================
    string loginId;
    string userRole;
    bool authenticated = false;

    while (!authenticated) {
        cout << "\n========== CAMPUSCARE LOGIN ==========\n";
        cout << "Enter your ID (0 to exit): ";
        cin >> loginId;

        if (loginId == "0") {
            cout << "Exiting CampusCare...\n";
            return 0;
        }

        ifstream userFile("users.txt");

        if (!userFile.is_open()) {
            cout << "\nError: Could not open users.txt\n";
            return 1;
        }

        string line;
        string registeredId;
        string registeredRole;
        string registeredPassword;
        bool idFound = false;

        while (getline(userFile, line)) {
            istringstream row(line);

            row >> registeredId >> registeredRole;

            if (loginId == registeredId) {
                idFound = true;
                userRole = registeredRole;

                // Admin login requires a password
                if (registeredRole == "admin") {
                    row >> registeredPassword;

                    string adminPassword;

                    cout << "Enter admin password: ";
                    cin >> adminPassword;

                    if (adminPassword == registeredPassword) {
                        authenticated = true;
                    }
                    else {
                        cout << "\nIncorrect admin password!\n";
                    }
                }
               else if (registeredRole == "student") {
    if (!studentManager.studentExists(loginId)) {
        // First-time student: allow login to create a profile.
        authenticated = true;
    }
    else if (studentManager.isAccountActive(loginId)) {
        authenticated = true;
    }
    else {
        cout << "\nYour CampusCare account is inactive. "
             << "Please contact the administrator.\n";

        authenticated = false;
    }
}
else if (registeredRole == "faculty") {
    int accountStatus = 0;
    row >> accountStatus;

    if (accountStatus == 1) {
        authenticated = true;
    }
    else {
        cout << "\nYour faculty account is inactive. "
             << "Please contact the administrator.\n";

        authenticated = false;
    }
}

                break;
            }
        }

        userFile.close();

        if (!idFound) {
            cout << "\nInvalid ID! Please try again.\n";
        }
    }

    cout << "\nLogin successful!\n";
    cout << "Welcome to CampusCare, " << userRole << "!\n";
    // ================= LOGIN COMPLETE =================

    // ================= STUDENT PROFILE CHECK =================

if (userRole == "student") {

    if (!studentManager.studentExists(loginId)) {

        cout << "\nNo student profile found for your account.\n";
        cout << "Let's create your student profile first.\n";

        studentManager.addStudent(loginId);
    }
}

// ================= PROFILE CHECK COMPLETE =================

    // Display unread notifications automatically after login
notificationManager.displayUnreadNotifications(
    loginId,
    userRole
);

// ================= EXISTING 22-FEATURE MENU =================




auto hasAccess = [&](int option) {
    if (userRole == "admin") {
        return true;  // Admin can access everything
    }

    if (userRole == "faculty") {
       return (option >= 1 && option <= 16) ||
           option == 22 ||
           option == 24 ||
           option == 25 ||
           option == 26 ||
           option == 28 ||
           option == 29 ||
           option == 31 ||
           option == 32 ||
           option == 38 ||
           option == 39;
           
    }

   if (userRole == "student") {
    return option == 2 || option == 3 ||
           option == 5 || option == 13 || option == 14 ||
           option == 23 || option == 25 || option == 26 ||
           option == 28 || option == 31 || option == 32 ||
           (option >= 33 && option <= 37);
}

    return false;
};

    do {

       
cout << "\n=====================================\n";
cout << "          CAMPUSCARE\n";
cout << "   Student Safety System\n";
cout << "=====================================\n";
cout << "Logged in as: " << userRole << "\n";

if (userRole == "student") {
    cout << "\n--- STUDENT MENU ---\n";
    cout << "2. Display Emergency Contacts\n";
    cout << "3. Search Emergency Contact\n";
    cout << "5. Add Incident\n";
    cout << "13. Show Campus Locations & Connections\n";
    cout << "14. Show Shortest Route Between Two Locations\n";
    cout << "23. My Incident Reports\n";
    cout << "25. Submit Confidential Concern\n";
    cout << "26. My Confidential Concerns\n";
    cout << "28. View My Messages" << endl;
    cout << "31. View My Notifications\n";
    cout << "32. Mark Notification as Read\n";
    cout << "\n--- STUDENT PROFILE ---\n";
    cout << "33. View My Profile\n";
    cout << "34. Update My Profile\n";
    cout << "35. Add Emergency Contact to My Profile\n";
    cout << "36. View My Profile Emergency Contacts\n";
    cout << "37. Delete My Profile Emergency Contact\n";
}

else if (userRole == "faculty") {
    cout << "\n--- FACULTY MENU ---\n";
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

    cout << "\n--- AMBULANCE DISPATCH ---\n";
    cout << "14. Show Shortest Route Between Two Locations\n";
    cout << "15. Display Ambulance Fleet & Status\n";
    cout << "16. Automatically Dispatch Ambulance to Incident\n";
    cout << "22. Sort Incidents\n";
    cout<<"24. update incident status\n";
    cout << "25. View Assigned Confidential Concerns\n";
    cout << "26. Update Confidential Concern Status\n";
    cout << "28. Send Message to Assigned Student" << endl;
    cout << "29. View My Messages" << endl;
    cout << "31. View My Notifications\n";
    cout << "32. Mark Notification as Read\n";
    cout << "38. View All Student Profiles\n";
    cout << "39. Search Student Profile\n";
}

else if (userRole == "admin") {
    cout << "\n--- ADMIN MENU ---\n";
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

    cout << "\n--- AMBULANCE DISPATCH ---\n";
    cout << "14. Show Shortest Route Between Two Locations\n";
    cout << "15. Display Ambulance Fleet & Status\n";
    cout << "16. Automatically Dispatch Ambulance to Incident\n";
    cout << "17. Update Ambulance Availability\n";
    cout << "18. Complete Dispatch\n";
    cout << "19. View Dispatch Log\n";
    cout << "20. BFS Traversal\n";
    cout << "21. DFS Traversal\n";
    cout << "22. Sort Incidents\n";
    cout << "23. My incidents report\n";
    cout<< "24. update incident status\n";
    cout << "25. View Assigned Confidential Concerns\n";
    cout << "26. Update Confidential Concern Status\n";
    cout << "27. View All Confidential Concerns\n";
    cout << "28. Send Message to Recipient" << endl;
    cout << "29. View My Messages" << endl;
    cout << "30. View Admin Dashboard" << endl;
    cout << "31. View My Notifications\n";
    cout << "32. Mark Notification as Read\n";
    cout << "38. View All Student Profiles\n";
    cout << "39. Search Student Profile\n";
    cout << "\n--- ACCOUNT MANAGEMENT ---\n";
    cout << "40. Deactivate User Account\n";
    cout << "41. Reactivate User Account\n";
    cout << "42. Register New User\n";
    
   
     

}

cout << "0. Exit\n";
cout << "\nEnter your choice: ";
cin >> choice;

if (choice != 0 && !hasAccess(choice)) {
    cout << "\nAccess Denied! You don't have permission to use this feature.\n";
}
else {

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

    Incident incident = incidentManager.addIncident(loginId);

    emergencySystem.addIncident(incident);

    // Send notifications when a critical incident is reported
    if (incident.severity == 4) {

        string alertMessage =
            "A critical incident has been reported at " +
            incident.location + ". Incident ID: " +
            to_string(incident.id);

        notificationManager.notifyRole(
            "student",
            incident.id,
            "Critical Alert",
            alertMessage
        );

        notificationManager.notifyRole(
            "faculty",
            incident.id,
            "Critical Alert",
            alertMessage
        );

        notificationManager.notifyRole(
            "admin",
            incident.id,
            "Critical Alert",
            alertMessage
        );

        cout << "\nCRITICAL ALERT: Notifications sent to all users!\n";
    }

    break;
}


            case 6:
                incidentManager.displayIncidents();
                break;

            case 7:
                incidentManager.searchIncident();
                break;

            case 8:
               {
                int deletedId = incidentManager.deleteIncident();

                     if (deletedId != -1) {
                             emergencySystem.removeIncident(deletedId);
    }

    break;
}
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

            case 14: {
                string from, to;
                cout << "Enter start location (exact name): ";
                getline(cin >> ws, from);
                cout << "Enter destination location (exact name): ";
                getline(cin, to);
                vector<string> route = campus.shortestPath(from, to);
                if (route.empty()) {
                    cout << "No route exists. One or both locations may be unknown or disconnected.\n";
                } else {
                    cout << "Shortest route: ";
                    for (size_t i = 0; i < route.size(); ++i) {
                        if (i) cout << " -> ";
                        cout << route[i];
                    }
                    cout << "\nTotal distance: " << campus.shortestDistance(from, to) << " m\n";
                }
                break;
            }

            case 15:
                ambulanceDispatch.displayAmbulances();
                break;

            case 16:
              {
    int dispatchedIncidentId = ambulanceDispatch.dispatchIncident();

    if (dispatchedIncidentId != -1) {

        for (const Incident& incident : incidentManager.getIncidents()) {
            if (incident.id == dispatchedIncidentId &&
                !incident.ownerId.empty()) {

                notificationManager.notifyUser(
                    incident.ownerId,
                    "student",
                    incident.id,
                    "Ambulance Dispatched",
                    "An ambulance has been dispatched for your incident. Incident ID: " +
                    to_string(incident.id)
                );

                break;
            }
        }

        notificationManager.notifyRole(
            "admin",
            dispatchedIncidentId,
            "Ambulance Dispatched",
            "An ambulance has been dispatched for incident ID: " +
            to_string(dispatchedIncidentId)
        );
    }

    break;
}
                break;

            case 17:
                ambulanceDispatch.updateAmbulanceStatus();
                break;

            case 18:
                {
    int completedIncidentId = ambulanceDispatch.completeDispatch();

    if (completedIncidentId != -1) {

        for (const Incident& incident : incidentManager.getIncidents()) {
            if (incident.id == completedIncidentId &&
                !incident.ownerId.empty()) {

                notificationManager.notifyUser(
                    incident.ownerId,
                    "student",
                    incident.id,
                    "Dispatch Completed",
                    "The ambulance dispatch for your incident has been marked as complete. Incident ID: " +
                    to_string(incident.id)
                );

                break;
            }
        }

        notificationManager.notifyRole(
            "admin",
            completedIncidentId,
            "Dispatch Completed",
            "Ambulance dispatch completed for incident ID: " +
            to_string(completedIncidentId)
        );
    }

    break;
}
            case 19:
                ambulanceDispatch.displayDispatchLog();
                break;

           
          
            case 20: {
                string start;

                cout << "Enter starting location (exact name): ";
                getline(cin >> ws, start);

                vector<string> result = campus.BFS(start);

                if (result.empty()) {
                    cout << "Invalid starting location!\n";
                }
                else {
                    cout << "BFS Traversal: ";

                    for (size_t i = 0; i < result.size(); ++i) {
                        if (i > 0) cout << " -> ";
                        cout << result[i];
                    }

                    cout << '\n';
                }

                break;
            }

            
            case 21: {
                string start;

                cout << "Enter starting location (exact name): ";
                getline(cin >> ws, start);

                vector<string> result = campus.DFS(start);

                if (result.empty()) {
                    cout << "Invalid starting location!\n";
                }
                else {
                    cout << "DFS Traversal: ";

                    for (size_t i = 0; i < result.size(); ++i) {
                        if (i > 0) cout << " -> ";
                        cout << result[i];
                    }

                    cout << '\n';
                }

                break;
            }
            case 22: {
    int sortChoice;

    cout << "\n===== SORT INCIDENT REPORTS =====\n";
    cout << "1. Sort by Severity\n";
    cout << "2. Sort by Incident ID (Newest First)\n";
    cout << "3. Sort by Location (Alphabetical)\n";
    cout << "Choose: ";

    if (!(cin >> sortChoice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice.\n";
        break;
    }

    switch (sortChoice) {
        case 1:
            incidentManager.sortIncidentsBySeverity();
            break;

        case 2:
            incidentManager.sortIncidentsById();
            break;

        case 3:
            incidentManager.sortIncidentsByLocation();
            break;

        default:
            cout << "Invalid sorting option.\n";
    }

    break;
}


case 23:
    incidentManager.displayMyReports(loginId);
    break;
case 24:
    {
    Incident updatedIncident;

    if (incidentManager.updateIncidentStatus(updatedIncident)) {

        string message =
            "Your incident report (ID: " +
            to_string(updatedIncident.id) +
            ") status has been updated to: " +
            updatedIncident.status;

        if (!updatedIncident.ownerId.empty()) {
            notificationManager.notifyUser(
                updatedIncident.ownerId,
                "student",
                updatedIncident.id,
                "Incident Status Update",
                message
            );
        }
    }

    break;
}
case 25:

    if (userRole == "student") {
        concernManager.submitConcern(loginId, loginId);
    }
    else if (userRole == "faculty") {
        concernManager.displayAssignedConcerns(loginId);
    }

    break;


case 26:

    if (userRole == "student") {
        concernManager.displayMyConcerns(loginId);
    }
    else if (userRole == "faculty") {
        int concernId;

        cout << "\nEnter Concern ID to update: ";
        cin >> concernId;

        concernManager.updateConcernStatus(
            concernId,
            loginId,
            false
        );
    }
    else if (userRole == "admin") {
        int concernId;

        cout << "\nEnter Concern ID to update: ";
        cin >> concernId;

        concernManager.updateConcernStatus(
            concernId,
            loginId,
            true
        );
    }

    break;


case 27:

    if (userRole == "admin") {
        concernManager.displayAllConcerns();
    }

    break; 
    
case 28:
    if (userRole == "student")
    {
        messageManager.displayMyMessages(loginId, userRole);
    }
    else if (userRole == "faculty")
    {
        messageManager.sendFacultyMessage(loginId);
    }
    else if (userRole == "admin")
    {
        messageManager.sendAdminMessage(loginId);
    }
    break;

case 29:
    if (userRole == "faculty")
    {
        messageManager.displayMyMessages(loginId, userRole);
    }
    else if (userRole == "admin")
    {
        messageManager.displayAllMessages();
    }
    break;


case 30:
    if (userRole == "admin") {
        displayAdminDashboard(incidentManager, ambulanceDispatch);
    }
    break;    


case 31:
    notificationManager.displayMyNotifications(
        loginId,
        userRole
    );

    cout << "\nUnread Notifications: "
         << notificationManager.getUnreadCount(
                loginId,
                userRole
            )
         << '\n';

    break;

case 32:
{
    int notificationId;

    cout << "\nEnter Notification ID to mark as read: ";
    cin >> notificationId;

    notificationManager.markNotificationAsRead(
        notificationId,
        loginId
    );

    break;
}
case 33:
    studentManager.displayStudentProfile(loginId);
    break;

case 34:
    studentManager.updateStudentProfile(loginId);
    break;

case 35:
    studentManager.addEmergencyContact(loginId);
    break;

case 36:
    studentManager.displayEmergencyContacts(loginId);
    break;

case 37:
    studentManager.deleteEmergencyContact(loginId);
    break;
case 38:
    studentManager.displayAllStudentProfiles();
    break;

case 39:
    studentManager.searchStudentProfile();
    break;

case 40:
case 41: {
    bool activate = (choice == 41);

    int userType;
    string userId;

    cout << "\n===== ACCOUNT MANAGEMENT =====\n";
    cout << "1. Student\n";
    cout << "2. Faculty\n";
    cout << "0. Back\n";
    cout << "Select user type: ";
    cin >> userType;

    if (userType == 0) {
        break;
    }

    if (userType != 1 && userType != 2) {
        cout << "\nInvalid user type!\n";
        break;
    }

    cout << "Enter User ID: ";
    cin >> userId;

    if (userType == 1) {
        if (activate) {
            studentManager.reactivateAccount(userId);
        }
        else {
            studentManager.deactivateAccount(userId);
        }
    }
    else {
        updateFacultyAccountStatus(userId, activate);
    }

    break;
}
    

case 42: {
    int userType;
    string userId;

    cout << "\n===== REGISTER NEW USER =====\n";
    cout << "1. Student\n";
    cout << "2. Faculty\n";
    cout << "0. Back\n";
    cout << "Select user type: ";
    cin >> userType;

    if (userType == 0) {
        break;
    }

    if (userType != 1 && userType != 2) {
        cout << "\nInvalid user type!\n";
        break;
    }

    cout << "Enter User ID: ";
    cin >> userId;

    if (userType == 1) {
        registerUser(userId, "student");
    }
    else {
        registerUser(userId, "faculty");
    }

    break;
}

case 0:
            
        contactManager.saveData();
                cout << "\nExiting CampusCare...\n";
                break;


           default:
    cout << "\nInvalid choice!\n";
    }  // Closes switch

}  // Closes permission-checking else

} while (choice != 0);  // Closes do-while loop

return 0;
}   