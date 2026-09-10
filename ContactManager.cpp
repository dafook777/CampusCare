#include "ContactManager.h"
#include "FileManager.h"

#include <iostream>
#include <algorithm>

using namespace std;


ContactManager::ContactManager() {

    contacts = FileManager::loadContacts();
}


// ADD CONTACT
void ContactManager::addContact() {

    Contact contact;

    cout << "\n========== ADD CONTACT ==========\n";

    cout << "Enter contact name: ";
    cin.ignore();
    getline(cin, contact.name);

    cout << "Enter phone number: ";
    getline(cin, contact.number);

    cout << "Enter category: ";
    getline(cin, contact.category);

    contacts.push_back(contact);

    FileManager::saveContacts(contacts);

    cout << "\nContact added successfully!\n";
}


// DISPLAY CONTACTS
void ContactManager::displayContacts() {

    cout << "\n========== EMERGENCY CONTACTS ==========\n";

    if (contacts.empty()) {
        cout << "No contacts available.\n";
        return;
    }

    for (int i = 0; i < contacts.size(); i++) {

        cout << "\nContact " << i + 1 << endl;
        cout << "Name     : " << contacts[i].name << endl;
        cout << "Number   : " << contacts[i].number << endl;
        cout << "Category : " << contacts[i].category << endl;
    }
}


// SEARCH CONTACT
void ContactManager::searchContact() {

    string name;

    cout << "\nEnter contact name to search: ";
    cin.ignore();
    getline(cin, name);

    for (const Contact& contact : contacts) {

        if (contact.name == name) {

            cout << "\nContact Found!\n";
            cout << "Name     : " << contact.name << endl;
            cout << "Number   : " << contact.number << endl;
            cout << "Category : " << contact.category << endl;

            return;
        }
    }

    cout << "\nContact not found.\n";
}


// DELETE CONTACT
void ContactManager::deleteContact() {

    string name;

    cout << "\nEnter contact name to delete: ";
    cin.ignore();
    getline(cin, name);

    for (auto it = contacts.begin(); it != contacts.end(); it++) {

        if (it->name == name) {

            contacts.erase(it);

            FileManager::saveContacts(contacts);

            cout << "\nContact deleted successfully!\n";

            return;
        }
    }

    cout << "\nContact not found.\n";
}


// SAVE DATA
void ContactManager::saveData() {

    FileManager::saveContacts(contacts);
}