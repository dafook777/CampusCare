#ifndef AMBULANCESERVICE_H
#define AMBULANCESERVICE_H

#include <QString>
#include <QStringList>
#include <QVector>

struct AmbulanceItem {
    QString id;
    QString location;
    QString status; // "Available", "Busy"
    int incidentId = -1;
};

struct DispatchResult {
    bool success = false;
    QString ambulanceId;
    QString fromLocation;
    QString toLocation;
    QStringList route;
    double metres = 0.0;
    double etaMinutes = 0.0;
    QString errorMsg;
};

class AmbulanceService {
public:
    static QString ambulanceFilePath();
    static QVector<AmbulanceItem> loadAll();
    static bool saveAll(const QVector<AmbulanceItem>& list);
    static DispatchResult dispatchNearest(int incidentId, const QString& incidentLocation);
    static bool releaseAmbulance(const QString& ambulanceId);
};

#endif // AMBULANCESERVICE_H

