#ifndef CONTACTSERVICE_H
#define CONTACTSERVICE_H

#include <QString>
#include <QVector>

struct ContactItem {
    QString name;
    QString number;
    QString category;
};

class ContactService {
public:
    static QString filePath();
    static QVector<ContactItem> loadAll();
    static bool addContact(const ContactItem& c);
    static bool deleteContact(const QString& name);
};

#endif

