#include "NotificationService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

QString NotificationService::filePath()
{
    return DataPath::resolve("notifications.txt");
}

QVector<NotificationItem> NotificationService::loadAll()
{
    QVector<NotificationItem> list;
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        const QStringList p = line.split('|');
        if (p.size() >= 7) {
            NotificationItem n;
            n.notificationId = p[0].trimmed().toInt();
            n.recipientId = p[1].trimmed();
            n.recipientRole = p[2].trimmed();
            n.incidentId = p[3].trimmed().toInt();
            n.notificationType = p[4].trimmed();
            n.message = p[5].trimmed();
            n.isRead = (p[6].trimmed() == "1");
            list.append(n);
        }
    }
    return list;
}

QVector<NotificationItem> NotificationService::loadForUser(const QString& userId)
{
    auto all = loadAll();
    QVector<NotificationItem> result;
    for (const auto& n : all)
        if (n.recipientId == userId) result.append(n);
    return result;
}

static void saveAll(const QVector<NotificationItem>& list)
{
    QFile file(NotificationService::filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return;
    QTextStream out(&file);
    for (const auto& n : list)
        out << n.notificationId << "|" << n.recipientId << "|" << n.recipientRole << "|"
            << n.incidentId << "|" << n.notificationType << "|" << n.message << "|"
            << (n.isRead ? "1" : "0") << "\n";
}

bool NotificationService::markRead(int notificationId)
{
    auto list = loadAll();
    for (auto& n : list)
        if (n.notificationId == notificationId) { n.isRead = true; break; }
    saveAll(list);
    return true;
}

int NotificationService::countUnread(const QString& userId)
{
    auto list = loadForUser(userId);
    int count = 0;
    for (const auto& n : list)
        if (!n.isRead) ++count;
    return count;
}

