
#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

struct Student {
    std::string studentId;
    std::string name;
    std::string department;

    // Additional student details
    std::string phoneNumber;
    std::string bloodGroup;
    std::string gender;

    // Emergency contacts
    std::vector<std::string> emergencyContacts;

    // Account status
    bool isActive = true;
};

#endif