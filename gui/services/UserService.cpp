#include "UserService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <algorithm>

QString UserService::filePath()
{
    return DataPath::resolve("users.txt");
}

QVector<UserItem> UserService::loadAll()
{
    QVector<UserItem> list;
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        // Format: id role [password] (space-delimited)
        const QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
            UserItem u;
            u.userId = parts[0];
            u.role = parts[1];
            if (parts.size() >= 3) u.password = parts[2];
            list.append(u);
        }
    }
    return list;
}

static void saveAll(const QVector<UserItem>& list)
{
    QFile file(UserService::filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return;
    QTextStream out(&file);
    out << "\n";  // original file has leading blank line
    for (const auto& u : list) {
        out << u.userId << " " << u.role;
        if (!u.password.isEmpty()) out << " " << u.password;
        out << "\n";
    }
}

bool UserService::addUser(const UserItem& u)
{
    auto list = loadAll();
    for (const auto& existing : list)
        if (existing.userId == u.userId) return false;  // already exists
    list.append(u);
    saveAll(list);
    return true;
}

bool UserService::deleteUser(const QString& userId)
{
    auto list = loadAll();
    list.erase(std::remove_if(list.begin(), list.end(),
        [&](const UserItem& u){ return u.userId == userId; }), list.end());
    saveAll(list);
    return true;
}

bool UserService::changeRole(const QString& userId, const QString& newRole)
{
    auto list = loadAll();
    for (auto& u : list)
        if (u.userId == userId) { u.role = newRole; break; }
    saveAll(list);
    return true;
}

