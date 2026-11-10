
#include "NotificationManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Constructor
NotificationManager::NotificationManager()
{
    loadNotifications();
}

// Load notifications from notifications.txt
void NotificationManager::loadNotifications()
{
    notifications.clear();

    ifstream file("notifications.txt");

    if (!file.is_open())
    {
        return;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string field;

        Notification notification;

        try
        {
            getline(ss, field, '|');
            notification.notificationId = stoi(field);

            getline(ss, notification.recipientId, '|');
            getline(ss, notification.recipientRole, '|');

            getline(ss, field, '|');
            notification.incidentId = stoi(field);

            getline(ss, notification.notificationType, '|');
            getline(ss, notification.message, '|');

            getline(ss, field);
            notification.isRead = (field == "1");

            notifications.push_back(notification);
        }
        catch (...)
        {
            // Ignore malformed records.
        }
    }

    file.close();
}

// Save notifications to notifications.txt
void NotificationManager::saveNotifications()
{
    ofstream file("notifications.txt");

    if (!file.is_open())
    {
        cout << "Error opening notifications.txt for writing.\n";
        return;
    }

    for (const Notification& notification : notifications)
    {
        file << notification.notificationId << "|"
             << notification.recipientId << "|"
             << notification.recipientRole << "|"
             << notification.incidentId << "|"
             << notification.notificationType << "|"
             << notification.message << "|"
             << (notification.isRead ? 1 : 0)
             << "\n";
    }

    file.close();
}

// Generate a unique notification ID
int NotificationManager::generateNotificationId()
{
    int maxId = 0;

    for (const Notification& notification : notifications)
    {
        if (notification.notificationId > maxId)
        {
            maxId = notification.notificationId;
        }
    }

    return maxId + 1;
}

// Send notification to one user
void NotificationManager::notifyUser(
    const string& recipientId,
    const string& recipientRole,
    int incidentId,
    const string& notificationType,
    const string& message
)
{
    if (recipientId.empty() || recipientRole.empty() || message.empty())
    {
        return;
    }

    Notification notification;

    notification.notificationId = generateNotificationId();
    notification.recipientId = recipientId;
    notification.recipientRole = recipientRole;
    notification.incidentId = incidentId;
    notification.notificationType = notificationType;
    notification.message = message;
    notification.isRead = false;

    notifications.push_back(notification);

    saveNotifications();
}

// Send notification to all users of a particular role
void NotificationManager::notifyRole(
    const string& role,
    int incidentId,
    const string& notificationType,
    const string& message
)
{
    ifstream file("users.txt");

    if (!file.is_open())
    {
        cout << "Error opening users.txt.\n";
        return;
    }

    string line;
    bool notificationCreated = false;

    while (getline(file, line))
    {
        stringstream ss(line);

        string userId;
        string userRole;

        ss >> userId >> userRole;

        if (userId.empty() || userRole != role)
        {
            continue;
        }

        Notification notification;

        notification.notificationId = generateNotificationId();
        notification.recipientId = userId;
        notification.recipientRole = userRole;
        notification.incidentId = incidentId;
        notification.notificationType = notificationType;
        notification.message = message;
        notification.isRead = false;

        notifications.push_back(notification);

        notificationCreated = true;
    }

    file.close();

    if (notificationCreated)
    {
        saveNotifications();
    }
}

// Display notifications belonging to a particular user
void NotificationManager::displayMyNotifications(
    const string& userId,
    const string& userRole
)
{
    bool found = false;

    cout << "\n========== MY NOTIFICATIONS ==========\n";

    for (const Notification& notification : notifications)
    {
        if (notification.recipientId == userId &&
            notification.recipientRole == userRole)
        {
            found = true;

            cout << "\nNotification ID : "
                 << notification.notificationId;

            cout << "\nType            : "
                 << notification.notificationType;

            if (notification.incidentId != 0)
            {
                cout << "\nIncident ID     : "
                     << notification.incidentId;
            }

            cout << "\nMessage         : "
                 << notification.message;

            cout << "\nStatus          : "
                 << (notification.isRead ? "Read" : "Unread");

            cout << "\n--------------------------------------\n";
        }
    }

    if (!found)
    {
        cout << "You have no notifications.\n";
    }
}



void NotificationManager::displayUnreadNotifications(
    const string& userId,
    const string& userRole
)
{
    int count = 0;

    for (const Notification& notification : notifications)
    {
        if (notification.recipientId == userId &&
            notification.recipientRole == userRole &&
            !notification.isRead)
        {
            if (count == 0)
            {
                cout << "\n========================================\n";
                cout << "          NEW NOTIFICATIONS\n";
                cout << "========================================\n";
            }

            count++;

            cout << "\n[NEW] " << notification.notificationType;
            cout << "\nMessage: " << notification.message;

            if (notification.incidentId != 0)
            {
                cout << "\nIncident ID: "
                     << notification.incidentId;
            }

            cout << "\n----------------------------------------\n";
        }
    }

    if (count > 0)
    {
        cout << "\nYou have " << count
             << " unread notification(s).\n";
    }
    else
    {
        cout << "\nYou have no new notifications.\n";
    }
}

// Mark a notification as read
void NotificationManager::markNotificationAsRead(
    int notificationId,
    const string& userId
)
{
    for (Notification& notification : notifications)
    {
        if (notification.notificationId == notificationId &&
            notification.recipientId == userId)
        {
            if (!notification.isRead)
            {
                notification.isRead = true;
                saveNotifications();

                cout << "Notification marked as read.\n";
            }
            else
            {
                cout << "Notification is already read.\n";
            }

            return;
        }
    }

    cout << "Notification not found or access denied.\n";
}

// Count unread notifications for a user
int NotificationManager::getUnreadCount(
    const string& userId,
    const string& userRole
) const
{
    int count = 0;

    for (const Notification& notification : notifications)
    {
        if (notification.recipientId == userId &&
            notification.recipientRole == userRole &&
            !notification.isRead)
        {
            count++;
        }
    }

    return count;
}