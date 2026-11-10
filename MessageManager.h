
#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "Message.h"
#include "ConcernManager.h"

#include <vector>
#include <string>

using namespace std;

class MessageManager
{
private:
    ConcernManager concernManager;
    vector<Message> messages;

    void loadMessages();
    void saveMessages();

    bool isValidUser(const string& userId,
                     const string& userRole);

    bool isFacultyAssignedToConcern(
        const string& facultyEmployeeId,
        int concernId);

public:
    MessageManager();

    void sendAdminMessage(const string& adminId);

    void sendFacultyMessage(
        const string& facultyEmployeeId);

    void displayMyMessages(
        const string& userId,
        const string& userRole);

    void displayAllMessages();

};

#endif
