#ifndef STUDENTSERVICE_H
#define STUDENTSERVICE_H

#include <QString>
#include <QVector>

struct StudentItem {
    QString studentId;
    QString name;
    QString department;
    QString phone;
    QString bloodGroup;
    QString gender;
    bool isActive = true;
};

class StudentService {
public:
    static QString filePath();
    static QVector<StudentItem> loadAll();
    static StudentItem getById(const QString& id);
    static bool update(const StudentItem& s);
};

#endif

