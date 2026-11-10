
#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "Notification.h"
#include <vector>
#include <string>

using namespace std;

class NotificationManager {
private:
    vector<Notification> notifications;

    // Load notifications from the text file
    void loadNotifications();

    // Save notifications to the text file
    void saveNotifications();

    // Generate a unique notification ID
    int generateNotificationId();

public:
    NotificationManager();

    // Send a notification to one specific user
    void notifyUser(
        const string& recipientId,
        const string& recipientRole,
        int incidentId,
        const string& notificationType,
        const string& message
    );

    // Send a notification to every user with a particular role
    void notifyRole(
        const string& role,
        int incidentId,
        const string& notificationType,
        const string& message
    );

    // Display notifications belonging to a user
    void displayMyNotifications(
        const string& userId,
        const string& userRole
    );

    void displayUnreadNotifications(
    const string& userId,
    const string& userRole
);

    // Mark a particular notification as read
    void markNotificationAsRead(
        int notificationId,
        const string& userId
    );

    // Count unread notifications for a user
    int getUnreadCount(
    const string& userId,
    const string& userRole
) const;
};

#endif
