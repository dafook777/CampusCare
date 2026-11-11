#ifndef CONCERNSERVICE_H
#define CONCERNSERVICE_H

#include <QString>
#include <QVector>

struct ConcernItem {
    int id = 0;
    QString studentId;
    QString studentName;
    QString facultyId;
    QString category;
    QString description;
    bool isAnonymous = false;
    QString status = "Open";
};

class ConcernService {
public:
    static QString filePath();
    static QVector<ConcernItem> loadAll();
    static bool addConcern(ConcernItem& item);
    static bool updateStatus(int id, const QString& newStatus);
    static bool deleteConcern(int id);
};

#endif

