
#include "ConcernManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

// Helper function to read non-empty text
static string readConcernText(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin >> ws, value);

        if (value.find_first_not_of(" \t") != string::npos) {
            return value;
        }

        cout << "Input cannot be empty. Try again.\n";
    }
}

// Constructor
ConcernManager::ConcernManager() {
    loadConcerns();
}

// Check whether Employee ID exists in faculty.txt
bool ConcernManager::isValidFaculty(const string& employeeId) {
    ifstream file("faculty.txt");

    if (!file.is_open()) {
        cout << "\nError: Could not open faculty.txt\n";
        return false;
    }

    string id;

    while (getline(file, id)) {
        if (id == employeeId) {
            return true;
        }
    }

    return false;
}

// Load concerns from concerns.txt
void ConcernManager::loadConcerns() {
    concerns.clear();

    ifstream file("concerns.txt");

    if (!file.is_open()) {
        return;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Concern concern;
        string idString;
        string anonymousString;

        getline(ss, idString, '|');
        getline(ss, concern.studentId, '|');
        getline(ss, concern.studentName, '|');
        getline(ss, concern.facultyEmployeeId, '|');
        getline(ss, concern.category, '|');
        getline(ss, concern.description, '|');
        getline(ss, anonymousString, '|');
        getline(ss, concern.status);

        try {
            concern.id = stoi(idString);
        }
        catch (const exception&) {
            cout << "Skipping invalid concern record.\n";
            continue;
        }

        concern.isAnonymous = (anonymousString == "1");

        if (concern.status.empty()) {
            concern.status = "Pending";
        }

        concerns.push_back(concern);
    }

    file.close();
}

// Save concerns to concerns.txt
void ConcernManager::saveConcerns() {
    ofstream file("concerns.txt");

    if (!file.is_open()) {
        cout << "\nError opening concerns.txt\n";
        return;
    }

    for (const Concern& concern : concerns) {
        file << concern.id << "|"
             << concern.studentId << "|"
             << concern.studentName << "|"
             << concern.facultyEmployeeId << "|"
             << concern.category << "|"
             << concern.description << "|"
             << (concern.isAnonymous ? "1" : "0") << "|"
             << concern.status << '\n';
    }

    file.close();
}

// Student submits a confidential concern
void ConcernManager::submitConcern(
    const string& studentId,
    const string& studentName
) {
    Concern concern;

    cout << "\n========== CONFIDENTIAL CONCERN ==========\n";

    // Generate concern ID
    int maxID = 2000;

    for (const Concern& existing : concerns) {
        if (existing.id > maxID) {
            maxID = existing.id;
        }
    }

    concern.id = maxID + 1;

    concern.studentId = studentId;
    concern.studentName = studentName;

    cout << "\nGenerated Concern ID: " << concern.id << '\n';

    // Choose faculty recipient
    while (true) {
        cout << "\nEnter Faculty Employee ID: ";
        getline(cin >> ws, concern.facultyEmployeeId);

        if (isValidFaculty(concern.facultyEmployeeId)) {
            cout << "Faculty ID verified.\n";
            break;
        }

        cout << "Invalid Employee ID. Please try again.\n";
    }

    concern.category = readConcernText(
        "\nEnter concern category: "
    );

    concern.description = readConcernText(
        "Describe your concern: "
    );

    // Anonymous option
    char choice;

    cout << "\nSubmit anonymously? (y/n): ";
    cin >> choice;

    concern.isAnonymous =
        (choice == 'y' || choice == 'Y');

    concern.status = "Pending";

    concerns.push_back(concern);

    saveConcerns();

    cout << "\nConcern submitted successfully!\n";
    cout << "Concern ID: " << concern.id << '\n';
    cout << "Status: " << concern.status << '\n';
}

// Student sees their own concerns
void ConcernManager::displayMyConcerns(
    const string& studentId
) {
    bool found = false;

    cout << "\n========== MY CONFIDENTIAL CONCERNS ==========\n";

    for (const Concern& concern : concerns) {
        if (concern.studentId == studentId) {
            found = true;

            cout << "\nConcern ID : " << concern.id;
            cout << "\nRecipient  : "
                 << concern.facultyEmployeeId;
            cout << "\nCategory   : " << concern.category;
            cout << "\nDescription: " << concern.description;
            cout << "\nStatus     : " << concern.status;
            cout << "\n----------------------------------------------\n";
        }
    }

    if (!found) {
        cout << "\nYou have not submitted any concerns.\n";
    }
}

// Faculty sees concerns addressed to their Employee ID
void ConcernManager::displayAssignedConcerns(
    const string& facultyEmployeeId
) {
    bool found = false;

    cout << "\n========== ASSIGNED CONFIDENTIAL CONCERNS ==========\n";

    for (const Concern& concern : concerns) {
        if (concern.facultyEmployeeId == facultyEmployeeId) {
            found = true;

            cout << "\nConcern ID : " << concern.id;

            if (!concern.isAnonymous) {
                cout << "\nStudent ID : " << concern.studentId;
                cout << "\nStudent    : " << concern.studentName;
            }
            else {
                cout << "\nStudent    : Anonymous";
            }

            cout << "\nCategory   : " << concern.category;
            cout << "\nDescription: " << concern.description;
            cout << "\nStatus     : " << concern.status;
            cout << "\n----------------------------------------------\n";
        }
    }

    if (!found) {
        cout << "\nNo concerns have been assigned to you.\n";
    }
}

// Admin sees all concerns
void ConcernManager::displayAllConcerns() {
    if (concerns.empty()) {
        cout << "\nNo confidential concerns recorded.\n";
        return;
    }

    cout << "\n========== ALL CONFIDENTIAL CONCERNS ==========\n";

    for (const Concern& concern : concerns) {
        cout << "\nConcern ID : " << concern.id;
        cout << "\nStudent ID : " << concern.studentId;
        cout << "\nStudent    : " << concern.studentName;
        cout << "\nFaculty ID : " << concern.facultyEmployeeId;
        cout << "\nCategory   : " << concern.category;
        cout << "\nDescription: " << concern.description;
        cout << "\nAnonymous  : "
             << (concern.isAnonymous ? "Yes" : "No");
        cout << "\nStatus     : " << concern.status;
        cout << "\n----------------------------------------------\n";
    }
}

// Update concern status
void ConcernManager::updateConcernStatus(
    int concernId,
    const string& facultyEmployeeId,
    bool isAdmin
) {
    for (Concern& concern : concerns) {
        if (concern.id == concernId) {

            // Faculty can update only their assigned concerns
            if (!isAdmin &&
                concern.facultyEmployeeId != facultyEmployeeId) {

                cout << "\nAccess denied. This concern is not assigned to you.\n";
                return;
            }

            cout << "\nCurrent Status: " << concern.status << '\n';

            cout << "\nChoose new status:\n";
            cout << "1. Pending\n";
            cout << "2. Under Review\n";
            cout << "3. In Progress\n";
            cout << "4. Resolved\n";
            cout << "Enter choice: ";

            int choice;

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                cout << "\nInvalid choice.\n";
                return;
            }

            switch (choice) {
                case 1:
                    concern.status = "Pending";
                    break;

                case 2:
                    concern.status = "Under Review";
                    break;

                case 3:
                    concern.status = "In Progress";
                    break;

                case 4:
                    concern.status = "Resolved";
                    break;

                default:
                    cout << "\nInvalid status choice.\n";
                    return;
            }

            saveConcerns();

            cout << "\nConcern status updated successfully!\n";
            cout << "New Status: " << concern.status << '\n';

            return;
        }
    }

    cout << "\nConcern not found.\n";

}  
bool ConcernManager::isFacultyAssignedToConcern(
    const string& facultyEmployeeId,
    int concernId)
{
    for (const Concern& concern : concerns)
    {
        if (concern.id == concernId &&
            concern.facultyEmployeeId == facultyEmployeeId)
        {
            return true;
        }
    }

    return false;
}

string ConcernManager::getConcernStudentId(int concernId)
{
    for (const Concern& concern : concerns)
    {
        if (concern.id == concernId)
        {
            return concern.studentId;
        }
    }

    return "";
}


