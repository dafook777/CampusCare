#ifndef CONTACTMANAGER_H
#define CONTACTMANAGER_H

#include "Contact.h"
#include <vector>
using namespace std;

class ContactManager {

private:
    vector<Contact> contacts;

public:

    ContactManager();

    void addContact();
    void displayContacts();
    void searchContact();
    void deleteContact();

    void saveData();
};

#endif