#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>
#include <QVector>

struct UserItem {
    QString userId;
    QString role;
    QString password;  // empty = no password
};

class UserService {
public:
    static QString filePath();
    static QVector<UserItem> loadAll();
    static bool addUser(const UserItem& u);
    static bool deleteUser(const QString& userId);
    static bool changeRole(const QString& userId, const QString& newRole);
};

#endif

