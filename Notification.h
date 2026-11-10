
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
using namespace std;

struct Notification {
    int notificationId;

    string recipientId;
    string recipientRole;

    int incidentId;

    string notificationType;
    string message;

    bool isRead;
};

#endif
