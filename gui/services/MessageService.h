#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#include <QString>
#include <QVector>

struct MessageItem {
    int messageId = 0;
    int concernId = 0;
    QString senderId;
    QString senderRole;
    QString recipientId;
    QString recipientRole;
    QString messageText;
};

class MessageService {
public:
    static QString filePath();
    static QVector<MessageItem> loadAll();
    static QVector<MessageItem> loadForUser(const QString& userId);
    static bool sendMessage(MessageItem& msg);
};

#endif

