#ifndef ROUTESERVICE_H
#define ROUTESERVICE_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>

struct RouteResult {
    QStringList path;
    double totalMetres = 0.0;
    double walkingMinutes = 0.0;
    bool found = false;
};

class RouteService {
public:
    static QStringList getNodes();
    static RouteResult findShortestDistance(const QString& from, const QString& to);
    static RouteResult findFewestHops(const QString& from, const QString& to);
    static RouteResult findNearestEmergency(const QString& from);

    struct Edge {
        QString u;
        QString v;
        double weight;
    };
    static QVector<Edge> getEdges();
};

#endif // ROUTESERVICE_H

