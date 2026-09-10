#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

using namespace std;

struct Incident {
    int id;
    string reporter;
    string location;
    string type;
    int severity;
    string description;
};

#endif