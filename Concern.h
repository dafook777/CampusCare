#ifndef CONCERN_H
#define CONCERN_H

#include <string>
using namespace std;

struct Concern {
    int id;

    // Student information
    string studentId;
    string studentName;

    // Faculty recipient
    string facultyEmployeeId;

    // Concern details
    string category;
    string description;

    // Privacy setting
    bool isAnonymous;

    // Concern progress
    string status;
};

#endif