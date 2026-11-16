#ifndef FACULTYSERVICE_H
#define FACULTYSERVICE_H

#include <QString>
#include <QVector>

struct FacultyItem {
    QString facultyId;
    QString name;
    QString department;
    QString phone;
    QString bloodGroup;
    QString gender;
    bool isActive = true;
    QString designation = "Associate Professor";
};

class FacultyService {
public:
    static QString filePath();
    static QVector<FacultyItem> loadAll();
    static FacultyItem getById(const QString& id);
    static bool update(const FacultyItem& f);
    static QString photoPath(const QString& facultyId);
    static bool savePhoto(const QString& facultyId, const QString& sourceImagePath);
};

#endif // FACULTYSERVICE_H
