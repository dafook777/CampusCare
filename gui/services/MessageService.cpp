#include "MessageService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

QString MessageService::filePath()
{
    return DataPath::resolve("messages.txt");
}

QVector<MessageItem> MessageService::loadAll()
{
    QVector<MessageItem> list;
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        const QStringList p = line.split('|');
        if (p.size() >= 7) {
            MessageItem m;
            m.messageId = p[0].trimmed().toInt();
            m.concernId = p[1].trimmed().toInt();
            m.senderId = p[2].trimmed();
            m.senderRole = p[3].trimmed();
            m.recipientId = p[4].trimmed();
            m.recipientRole = p[5].trimmed();
            m.messageText = p[6].trimmed();
            list.append(m);
        }
    }
    return list;
}

QVector<MessageItem> MessageService::loadForUser(const QString& userId)
{
    QVector<MessageItem> all = loadAll();
    QVector<MessageItem> result;
    for (const auto& m : all) {
        if (m.senderId == userId || m.recipientId == userId)
            result.append(m);
    }
    return result;
}

bool MessageService::sendMessage(MessageItem& msg)
{
    auto list = loadAll();
    int maxId = 0;
    for (const auto& m : list) if (m.messageId >= maxId) maxId = m.messageId + 1;
    msg.messageId = maxId;
    list.append(msg);

    QFile file(filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&file);
    for (const auto& m : list)
        out << m.messageId << "|" << m.concernId << "|" << m.senderId << "|"
            << m.senderRole << "|" << m.recipientId << "|" << m.recipientRole << "|"
            << m.messageText << "\n";
    return true;
}

