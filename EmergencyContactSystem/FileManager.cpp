#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

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