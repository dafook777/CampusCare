
#include "MessageManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

MessageManager::MessageManager()
{
    loadMessages();
}

void MessageManager::loadMessages()
{
    messages.clear();

    ifstream file("messages.txt");

    if (!file.is_open())
    {
        return;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string field;
        Message msg;

        try
        {
            getline(ss, field, '|');
            msg.messageId = stoi(field);

            getline(ss, field, '|');
            msg.concernId = stoi(field);

            getline(ss, msg.senderId, '|');
            getline(ss, msg.senderRole, '|');
            getline(ss, msg.recipientId, '|');
            getline(ss, msg.recipientRole, '|');
            getline(ss, msg.messageText);

            messages.push_back(msg);
        }
        catch (...)
        {
            // Skip malformed records.
        }
    }

    file.close();
}

void MessageManager::saveMessages()
{
    ofstream file("messages.txt");

    if (!file.is_open())
    {
        cout << "Error opening messages.txt for writing.\n";
        return;
    }

    for (const Message& msg : messages)
    {
        file << msg.messageId << "|"
             << msg.concernId << "|"
             << msg.senderId << "|"
             << msg.senderRole << "|"
             << msg.recipientId << "|"
             << msg.recipientRole << "|"
             << msg.messageText << "\n";
    }

    file.close();
}

bool MessageManager::isValidUser(
    const string& userId,
    const string& userRole)
{
    ifstream file("users.txt");

    if (!file.is_open())
    {
        return false;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);

        string storedId;
        string storedRole;

        ss >> storedId >> storedRole;

        if (storedId == userId &&
            storedRole == userRole)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool MessageManager::isFacultyAssignedToConcern(
    const string& facultyEmployeeId,
    int concernId)
{
    return concernManager.isFacultyAssignedToConcern(
        facultyEmployeeId,
        concernId);
}

void MessageManager::sendAdminMessage(
    const string& adminId)
{
    string recipientId;
    string recipientRole;
    string messageText;

    cout << "\n--- Send Admin Message ---\n";

    cout << "Enter recipient ID: ";
    cin >> recipientId;

    cout << "Enter recipient role (student/faculty): ";
    cin >> recipientRole;

    if (recipientRole != "student" &&
        recipientRole != "faculty")
    {
        cout << "Invalid recipient role.\n";
        return;
    }

    if (!isValidUser(recipientId, recipientRole))
    {
        cout << "Recipient does not exist.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter message: ";
    getline(cin, messageText);

    if (messageText.empty())
    {
        cout << "Message cannot be empty.\n";
        return;
    }

    if (messageText.find('|') != string::npos)
    {
        cout << "The | character is not allowed in messages.\n";
        return;
    }

    int nextId = 1;

    for (const Message& msg : messages)
    {
        nextId = max(nextId, msg.messageId + 1);
    }

    Message msg;

    msg.messageId = nextId;
    msg.concernId = 0;
    msg.senderId = adminId;
    msg.senderRole = "admin";
    msg.recipientId = recipientId;
    msg.recipientRole = recipientRole;
    msg.messageText = messageText;

    messages.push_back(msg);
    saveMessages();

    cout << "Admin message sent successfully.\n";
}

void MessageManager::sendFacultyMessage(
    const string& facultyEmployeeId)
{
    int concernId;
    string messageText;

    cout << "\n--- Send Faculty Message ---\n";

    cout << "Enter Concern ID: ";
    cin >> concernId;

    string studentId =
        concernManager.getConcernStudentId(concernId);

    if (studentId.empty())
    {
        cout << "Concern not found.\n";
        return;
    }

    if (!isFacultyAssignedToConcern(
            facultyEmployeeId, concernId))
    {
        cout << "You are not assigned to this concern.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter message: ";
    getline(cin, messageText);

    if (messageText.empty())
    {
        cout << "Message cannot be empty.\n";
        return;
    }

    if (messageText.find('|') != string::npos)
    {
        cout << "The | character is not allowed in messages.\n";
        return;
    }

    int nextId = 1;

    for (const Message& msg : messages)
    {
        nextId = max(nextId, msg.messageId + 1);
    }

    Message msg;

    msg.messageId = nextId;
    msg.concernId = concernId;
    msg.senderId = facultyEmployeeId;
    msg.senderRole = "faculty";
    msg.recipientId = studentId;
    msg.recipientRole = "student";
    msg.messageText = messageText;

    messages.push_back(msg);
    saveMessages();

    cout << "Faculty message sent successfully.\n";
}

void MessageManager::displayMyMessages(
    const string& userId,
    const string& userRole)
{
    cout << "\n--- My Messages ---\n";

    bool found = false;

    for (const Message& msg : messages)
    {
        if (msg.recipientId == userId &&
            msg.recipientRole == userRole)
        {
            found = true;

            cout << "\nMessage ID: " << msg.messageId;
            cout << "\nFrom: " << msg.senderId
                 << " (" << msg.senderRole << ")";

            if (msg.concernId != 0)
            {
                cout << "\nConcern ID: " << msg.concernId;
            }

            cout << "\nMessage: " << msg.messageText;
            cout << "\n-------------------------\n";
        }
    }

    if (!found)
    {
        cout << "No messages found.\n";
    }
}

void MessageManager::displayAllMessages()
{
    cout << "\n--- All CampusCare Messages ---\n";

    if (messages.empty())
    {
        cout << "No messages found.\n";
        return;
    }

    for (const Message& msg : messages)
    {
        cout << "\nMessage ID: " << msg.messageId;
        cout << "\nConcern ID: " << msg.concernId;

        cout << "\nFrom: " << msg.senderId
             << " (" << msg.senderRole << ")";

        cout << "\nTo: " << msg.recipientId
             << " (" << msg.recipientRole << ")";

        cout << "\nMessage: " << msg.messageText;
        cout << "\n-------------------------\n";
    }
}
