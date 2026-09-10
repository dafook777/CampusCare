#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Contact.h"
#include "incident.h"
#include <vector>
using namespace std;

class FileManager {
public:
    static void saveContacts(const vector<Contact>& contacts);
    static vector<Contact> loadContacts();

    static void saveIncidents(const vector<Incident>& incidents);
    static vector<Incident> loadIncidents();
};

#endif