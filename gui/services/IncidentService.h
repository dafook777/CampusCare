#ifndef INCIDENTSERVICE_H
#define INCIDENTSERVICE_H

#include <QString>
#include <QVector>

struct IncidentItem {
    int id = 0;
    QString reporter;
    QString location;
    QString type;
    int severity = 1;     // 1=Low, 2=Medium, 3=High, 4=Critical
    QString description;
    QString ownerId;
    QString status = "Open"; // "Open", "Assigned", "Resolved"
};

class IncidentService {
public:
    static QString incidentsFilePath();
    static QVector<IncidentItem> loadAll();
    static QVector<IncidentItem> loadPriorityQueue();
    static bool addIncident(const IncidentItem& item);
    static bool updateStatus(int id, const QString& newStatus);
    static bool deleteIncident(int id);
    static IncidentItem getById(int id);
};

#endif // INCIDENTSERVICE_H
