
#ifndef CONCERNMANAGER_H
#define CONCERNMANAGER_H

#include "Concern.h"
#include <vector>
#include <string>

using namespace std;

class ConcernManager {
private:
    vector<Concern> concerns;

    // Load and save concerns
    void loadConcerns();
    void saveConcerns();

    // Validate faculty Employee ID
    bool isValidFaculty(const string& employeeId);

public:
    ConcernManager();

    // Student functions
    void submitConcern(
        const string& studentId,
        const string& studentName
    );


    bool isFacultyAssignedToConcern(
    const string& facultyEmployeeId,
    int concernId);

    string getConcernStudentId(int concernId);

    
    void displayMyConcerns(const string& studentId);

    // Faculty functions
    void displayAssignedConcerns(
        const string& facultyEmployeeId
    );

    // Admin functions
    void displayAllConcerns();

    // Update concern status
    void updateConcernStatus(
        int concernId,
        const string& facultyEmployeeId,
        bool isAdmin
    );
};

#endif
