
#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "Student.h"
#include <string>
#include <vector>

class StudentManager {
private:
    std::vector<Student> students;
    const std::string studentFile = "students.txt";

    void loadStudents();
    void saveStudents() const;

    int findStudentIndex(const std::string& studentId) const;

public:
    StudentManager();

    
bool isAccountActive(const std::string& studentId) const;

    // Student profile operations
    bool addStudent(const std::string& studentId);
    void displayStudentProfile(const std::string& studentId) const;
    bool updateStudentProfile(const std::string& studentId);

    void displayAllStudentProfiles() const;
    void searchStudentProfile() const;

    // Emergency contact operations
    bool addEmergencyContact(const std::string& studentId);
    bool displayEmergencyContacts(const std::string& studentId) const;
    bool deleteEmergencyContact(const std::string& studentId);

    // Account management
    bool changePassword(const std::string& studentId);
    bool reactivateAccount(const std::string& studentId);
    bool deactivateAccount(const std::string& studentId);

    // Student lookup
    bool studentExists(const std::string& studentId) const;
};

#endif
