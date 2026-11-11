#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <QString>
#include <QVector>

struct NotificationItem {
    int notificationId = 0;
    QString recipientId;
    QString recipientRole;
    int incidentId = 0;
    QString notificationType;
    QString message;
    bool isRead = false;
};

class NotificationService {
public:
    static QString filePath();
    static QVector<NotificationItem> loadAll();
    static QVector<NotificationItem> loadForUser(const QString& userId);
    static bool markRead(int notificationId);
    static int countUnread(const QString& userId);
};

#endif

