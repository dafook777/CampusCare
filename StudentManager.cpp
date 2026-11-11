
#include "StudentManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

// Constructor: load existing student records when manager is created
StudentManager::StudentManager() {
    loadStudents();
}

// Find student using their ID
int StudentManager::findStudentIndex(const string& studentId) const {
    for (int i = 0; i < students.size(); i++) {
        if (students[i].studentId == studentId) {
            return i;
        }
    }

    return -1;
}

// Check whether a student exists
bool StudentManager::studentExists(const string& studentId) const {
    return findStudentIndex(studentId) != -1;
}

bool StudentManager::isAccountActive(const string& studentId) const {
    int index = findStudentIndex(studentId);

    if (index == -1) {
        return false;
    }

    return students[index].isActive;
}

// Load student records from file
void StudentManager::loadStudents() {
    students.clear();

    ifstream file(studentFile);

    if (!file.is_open()) {
        return;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Student student;

        string active;
        string contacts;

        getline(ss, student.studentId, '|');
        getline(ss, student.name, '|');
        getline(ss, student.department, '|');
        getline(ss, student.phoneNumber, '|');
        getline(ss, student.bloodGroup, '|');
        getline(ss, student.gender, '|');
        getline(ss, active, '|');
        getline(ss, contacts);

        if (student.studentId.empty()) {
            continue;
        }

        student.isActive = (active == "1");

        stringstream contactStream(contacts);
        string contact;

        while (getline(contactStream, contact, ';')) {
            if (!contact.empty()) {
                student.emergencyContacts.push_back(contact);
            }
        }

        students.push_back(student);
    }

    file.close();
}

// Register a new student


// Save all student records to file
void StudentManager::saveStudents() const {
    ofstream file(studentFile);

    if (!file.is_open()) {
        cout << "Error: Unable to save student records.\n";
        return;
    }

    for (const Student& student : students) {
        file << student.studentId << '|'
             << student.name << '|'
             << student.department << '|'
             << student.phoneNumber << '|'
             << student.bloodGroup << '|'
             << student.gender << '|'
             << (student.isActive ? "1" : "0") << '|';

        for (size_t i = 0; i < student.emergencyContacts.size(); i++) {
            file << student.emergencyContacts[i];

            if (i < student.emergencyContacts.size() - 1) {
                file << ';';
            }
        }

        file << '\n';
    }

    file.close();
}

 
bool StudentManager::addStudent(const string& studentId) {
    Student student;

    cout << "\n========== STUDENT PROFILE SETUP ==========\n";

    student.studentId = studentId;

    if (student.studentId.empty() ||
        student.studentId.find('|') != string::npos) {
        cout << "Invalid student ID.\n";
        return false;
    }

    if (studentExists(student.studentId)) {
        cout << "A profile already exists for this student.\n";
        return false;
    }

    cout << "Enter Student Name: ";
    getline(cin >> ws, student.name);

    cout << "Enter Department: ";
    getline(cin, student.department);

    cout << "Enter Phone Number (10 digits): ";
    cin >> student.phoneNumber;

    // Validate phone number
    if (student.phoneNumber.length() != 10 ||
        !all_of(student.phoneNumber.begin(),
                student.phoneNumber.end(),
                ::isdigit)) {
        cout << "Invalid phone number. Enter exactly 10 digits.\n";
        return false;
    }

    // Blood group selection
    int bloodChoice;

    cout << "\nSelect Blood Group:\n";
    cout << "1. A+\n";
    cout << "2. A-\n";
    cout << "3. B+\n";
    cout << "4. B-\n";
    cout << "5. AB+\n";
    cout << "6. AB-\n";
    cout << "7. O+\n";
    cout << "8. O-\n";
    cout << "9. Unknown\n";
    cout << "Enter choice: ";

    if (!(cin >> bloodChoice) ||
        bloodChoice < 1 || bloodChoice > 9) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid blood group choice.\n";
        return false;
    }

    const string bloodGroups[] = {
        "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-", "Unknown"
    };

    student.bloodGroup = bloodGroups[bloodChoice - 1];

    // Gender selection
    int genderChoice;

    cout << "\nSelect Gender:\n";
    cout << "1. Male\n";
    cout << "2. Female\n";
    cout << "Enter choice: ";

    if (!(cin >> genderChoice) ||
        genderChoice < 1 || genderChoice > 2) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid gender choice.\n";
        return false;
    }

    student.gender = (genderChoice == 1) ? "Male" : "Female";

    // Validate remaining profile details
    if (student.name.empty() ||
        student.department.empty() ||
        student.name.find('|') != string::npos ||
        student.department.find('|') != string::npos) {
        cout << "Invalid student details.\n";
        return false;
    }

    student.isActive = true;

    students.push_back(student);
    saveStudents();

    cout << "\nStudent profile created successfully!\n";

    return true;
}

// Display a student's profile
void StudentManager::displayStudentProfile(const string& studentId) const {
    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "\nStudent profile not found.\n";
        return;
    }

    const Student& student = students[index];

    cout << "\n====================================\n";
    cout << "         STUDENT PROFILE\n";
    cout << "====================================\n";

    cout << "Student ID   : " << student.studentId << '\n';
    cout << "Name         : " << student.name << '\n';
    cout << "Department   : " << student.department << '\n';
    cout << "Phone Number : " << student.phoneNumber << '\n';
    cout << "Blood Group  : " << student.bloodGroup << '\n';
    cout << "Gender       : " << student.gender << '\n';
    cout << "Account      : " << (student.isActive ? "Active" : "Inactive") << '\n';

    cout << "Emergency Contacts : "
         << student.emergencyContacts.size() << '\n';

    cout << "====================================\n";
}
// Update a student's profile
bool StudentManager::updateStudentProfile(const string& studentId) {
    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "\nStudent profile not found.\n";
        return false;
    }

    Student& student = students[index];

    cout << "\n========== UPDATE STUDENT PROFILE ==========\n";
    cout << "Press Enter to keep the existing value.\n";

    string input;

    cout << "Current Name: " << student.name << '\n';
    cout << "New Name: ";
    getline(cin >> ws, input);

    if (!input.empty()) {
        if (input.find('|') != string::npos) {
            cout << "Invalid name.\n";
            return false;
        }
        student.name = input;
    }

    cout << "\nCurrent Department: " << student.department << '\n';
    cout << "New Department: ";
    getline(cin, input);

    if (!input.empty()) {
        if (input.find('|') != string::npos) {
            cout << "Invalid department.\n";
            return false;
        }
        student.department = input;
    }

    cout << "\nCurrent Phone: " << student.phoneNumber << '\n';
    cout << "New Phone Number (10 digits): ";
    getline(cin, input);

    if (!input.empty()) {
        if (input.length() != 10 ||
            !all_of(input.begin(), input.end(),
                    [](unsigned char c) { return isdigit(c); })) {
            cout << "Invalid phone number.\n";
            return false;
        }
        student.phoneNumber = input;
    }

    cout << "\nCurrent Blood Group: " << student.bloodGroup << '\n';
    cout << "Update blood group? (y/n): ";

    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        const string bloodGroups[] = {
            "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-", "Unknown"
        };

        cout << "\nSelect Blood Group:\n";
        for (int i = 0; i < 9; i++) {
            cout << i + 1 << ". " << bloodGroups[i] << '\n';
        }

        int bloodChoice;
        cout << "Enter choice: ";

        if (!(cin >> bloodChoice) ||
            bloodChoice < 1 || bloodChoice > 9) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid blood group choice.\n";
            return false;
        }

        student.bloodGroup = bloodGroups[bloodChoice - 1];
    }

    cout << "\nCurrent Gender: " << student.gender << '\n';
    cout << "Update gender? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        int genderChoice;

        cout << "1. Male\n";
        cout << "2. Female\n";
        cout << "Enter choice: ";

        if (!(cin >> genderChoice) ||
            genderChoice < 1 || genderChoice > 2) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid gender choice.\n";
            return false;
        }

        student.gender = (genderChoice == 1) ? "Male" : "Female";
    }

    saveStudents();

    cout << "\nStudent profile updated successfully!\n";
    return true;
}

void StudentManager::displayAllStudentProfiles() const {
    cout << "\n========== ALL STUDENT PROFILES ==========\n";

    if (students.empty()) {
        cout << "No student profiles found.\n";
        return;
    }

    for (const Student& student : students) {
        cout << "\n----------------------------------\n";
        cout << "Student ID: " << student.studentId << '\n';
        cout << "Name: " << student.name << '\n';
        cout << "Department: " << student.department << '\n';
        cout << "Phone Number: " << student.phoneNumber << '\n';
        cout << "Blood Group: " << student.bloodGroup << '\n';
        cout << "Gender: " << student.gender << '\n';
        cout << "Account Status: "
             << (student.isActive ? "Active" : "Inactive") << '\n';
        cout << "Emergency Contacts: "
             << student.emergencyContacts.size() << '\n';
    }

    cout << "\n=========================================\n";
}

void StudentManager::searchStudentProfile() const {
    string studentId;

    cout << "\n========== SEARCH STUDENT PROFILE ==========\n";
    cout << "Enter Student ID: ";
    cin >> studentId;

    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "No student profile found with ID: "
             << studentId << '\n';
        return;
    }

    displayStudentProfile(studentId);
}

// Add an emergency contact
bool StudentManager::addEmergencyContact(const string& studentId) {
    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "\nStudent not found.\n";
        return false;
    }

    string name;
    string relationship;
    string phone;

    cout << "\n========== ADD EMERGENCY CONTACT ==========\n";

    cout << "Contact Name: ";
    getline(cin >> ws, name);

    cout << "Relationship: ";
    getline(cin, relationship);

    cout << "Phone Number: ";
    getline(cin, phone);

    if (name.empty() || relationship.empty() || phone.empty() ||
        name.find_first_of("|;") != string::npos ||
        relationship.find_first_of("|;") != string::npos ||
        phone.find_first_of("|;") != string::npos) {
        cout << "Invalid contact details.\n";
        return false;
    }

    string contact = name + " - " + relationship + " - " + phone;

    students[index].emergencyContacts.push_back(contact);

    saveStudents();

    cout << "\nEmergency contact added successfully!\n";

    return true;
}

// Display emergency contacts
bool StudentManager::displayEmergencyContacts(
    const string& studentId) const {

    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "\nStudent not found.\n";
        return false;
    }

    const Student& student = students[index];

    cout << "\n========== EMERGENCY CONTACTS ==========\n";

    if (student.emergencyContacts.empty()) {
        cout << "No emergency contacts saved.\n";
        return true;
    }

    for (int i = 0; i < student.emergencyContacts.size(); i++) {
        cout << i + 1 << ". "
             << student.emergencyContacts[i] << '\n';
    }

    return true;
}

// Delete an emergency contact
bool StudentManager::deleteEmergencyContact(const string& studentId) {
    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "\nStudent not found.\n";
        return false;
    }

    Student& student = students[index];

    if (student.emergencyContacts.empty()) {
        cout << "\nNo emergency contacts to delete.\n";
        return false;
    }

    displayEmergencyContacts(studentId);

    int choice;

    cout << "\nEnter contact number to delete: ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
        return false;
    }

    if (choice < 1 ||
        choice > static_cast<int>(student.emergencyContacts.size())) {
        cout << "Invalid contact number.\n";
        return false;
    }

    student.emergencyContacts.erase(
        student.emergencyContacts.begin() + choice - 1
    );

    saveStudents();

    cout << "\nEmergency contact deleted successfully!\n";

    return true;
}

// Password changing will be implemented with secure password hashing
bool StudentManager::changePassword(const string& studentId) {
    if (!studentExists(studentId)) {
        cout << "\nStudent not found.\n";
        return false;
    }

    cout << "\nPassword management is not enabled yet.\n";
    cout << "Secure password hashing must be integrated first.\n";

    return false;
}

// Deactivate a student account
bool StudentManager::deactivateAccount(const string& studentId) {
    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "\nStudent not found.\n";
        return false;
    }

    if (!students[index].isActive) {
        cout << "\nThis account is already inactive.\n";
        return false;
    }

    char confirmation;

    cout << "\nAre you sure you want to deactivate this account? (y/n): ";
    cin >> confirmation;

    if (confirmation != 'y' && confirmation != 'Y') {
        cout << "Account deactivation cancelled.\n";
        return false;
    }

    students[index].isActive = false;

    saveStudents();

    cout << "\nAccount deactivated successfully.\n";

    return true;
}

bool StudentManager::reactivateAccount(const string& studentId) {
    int index = findStudentIndex(studentId);

    if (index == -1) {
        cout << "\nStudent profile not found.\n";
        return false;
    }

    if (students[index].isActive) {
        cout << "\nThis student account is already active.\n";
        return false;
    }

    students[index].isActive = true;

    saveStudents();

    cout << "\nStudent account reactivated successfully!\n";
    return true;
}
