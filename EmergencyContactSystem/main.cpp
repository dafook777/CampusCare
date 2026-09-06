#include <iostream>
#include "ContactManager.h"

using namespace std;

int main() {

    ContactManager contactManager;

    int choice;

    do {

        cout << "\n=====================================\n";
        cout << "          CAMPUSCARE\n";
        cout << "   Student Safety System\n";
        cout << "=====================================\n";

        cout << "1. Add Emergency Contact\n";
        cout << "2. Display Emergency Contacts\n";
        cout << "3. Search Emergency Contact\n";
        cout << "4. Delete Emergency Contact\n";
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