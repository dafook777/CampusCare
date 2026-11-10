
#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

using namespace std;


// ================= CONTACT FUNCTIONS =================

void FileManager::saveContacts(const vector<Contact>& contacts) {

    ofstream file("contacts.txt");

    if (!file) {
        cout << "Error opening contacts file for writing.\n";
        return;
    }

    for (const Contact& contact : contacts) {
        file << contact.name << "|"
             << contact.number << "|"
             << contact.category << endl;
    }

    file.close();
}


vector<Contact> FileManager::loadContacts() {

    vector<Contact> contacts;

    ifstream file("contacts.txt");

    if (!file) {
        cout << "No contacts file found. Starting with empty contacts.\n";
        return contacts;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        Contact contact;

        getline(ss, contact.name, '|');
        getline(ss, contact.number, '|');
        getline(ss, contact.category, '|');

        contacts.push_back(contact);
    }

    file.close();

    return contacts;
}


// ================= INCIDENT FUNCTIONS =================

void FileManager::saveIncidents(const vector<Incident>& incidents) {

    ofstream file("incidents.txt");

    if (!file) {
        cout << "Error opening incidents file for writing.\n";
        return;
    }

    for (const Incident& incident : incidents) {

        file << incident.id << "|"
             << incident.reporter << "|"
             << incident.location << "|"
             << incident.type << "|"
             << incident.severity << "|"
             << incident.description << "|"
             << incident.ownerId << "|"
             << incident.status << endl;
    }

    file.close();
}


vector<Incident> FileManager::loadIncidents() {

    vector<Incident> incidents;

    ifstream file("incidents.txt");

    if (!file) {
        return incidents;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        Incident incident;

        string idString;
        string severityString;

        getline(ss, idString, '|');
        getline(ss, incident.reporter, '|');
        getline(ss, incident.location, '|');
        getline(ss, incident.type, '|');
        getline(ss, severityString, '|');
        getline(ss, incident.description, '|');

        // Load owner ID if it exists.
        getline(ss, incident.ownerId, '|');

        // Load status if it exists.
        getline(ss, incident.status);

        if (incident.status.empty()) {
            incident.status = "Pending";
        }

        try {
            incident.id = stoi(idString);
            incident.severity = stoi(severityString);
        }
        catch (const exception&) {
            cout << "Skipping invalid incident record.\n";
            continue;
        }

        incidents.push_back(incident);
    }

    file.close();

    return incidents;
}
