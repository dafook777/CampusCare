#include "AmbulanceService.h"
#include "IncidentService.h"
#include "RouteService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <limits>

QString AmbulanceService::ambulanceFilePath()
{
    return DataPath::resolve("ambulances.txt");
}

QVector<AmbulanceItem> AmbulanceService::loadAll()
{
    QVector<AmbulanceItem> list;
    QFile file(ambulanceFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;

    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        const QStringList parts = line.split('|');
        if (parts.size() >= 4) {
            AmbulanceItem amb;
            amb.id = parts[0];
            amb.location = parts[1];
            amb.status = parts[2];
            amb.incidentId = parts[3].toInt();
            list.append(amb);
        }
    }
    return list;
}

bool AmbulanceService::saveAll(const QVector<AmbulanceItem>& list)
{
    QFile file(ambulanceFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;

    QTextStream out(&file);
    for (const auto& a : list) {
        out << a.id << '|'
            << a.location << '|'
            << a.status << '|'
            << a.incidentId << '\n';
    }
    return true;
}

static QString toGraphNode(const QString& loc)
{
    const auto nodes = RouteService::getNodes();
    for (const auto& n : nodes) {
        if (n.compare(loc.trimmed(), Qt::CaseInsensitive) == 0) return n;
    }
    // common aliases
    const QString l = loc.trimmed().toLower();
    if (l.contains("main gate")) return "Main gate";
    if (l.contains("hostel")) return "Girls hostel";
    if (l.contains("security")) return "Gate 1";
    if (l.contains("library")) return "Open Audi";
    if (l.contains("geu") || l.contains("ground")) return "Gehu ground";
    if (l.contains("cafeteria")) return "Cafeteria";
    if (l.contains("cse")) return "CSE dept";
    return "Main gate";
}

DispatchResult AmbulanceService::dispatchNearest(int incidentId, const QString& incidentLocation)
{
    DispatchResult res;
    auto ambulances = loadAll();
    const QString targetNode = toGraphNode(incidentLocation);

    int bestIdx = -1;
    double bestDist = std::numeric_limits<double>::infinity();
    RouteResult bestRoute;

    for (int i = 0; i < ambulances.size(); ++i) {
        if (ambulances[i].status.compare("Available", Qt::CaseInsensitive) != 0) continue;

        const QString ambNode = toGraphNode(ambulances[i].location);
        auto r = RouteService::findShortestDistance(ambNode, targetNode);
        if (r.found && r.totalMetres < bestDist) {
            bestDist = r.totalMetres;
            bestIdx = i;
            bestRoute = r;
        }
    }

    if (bestIdx == -1) {
        res.errorMsg = "No available ambulance found.";
        return res;
    }

    ambulances[bestIdx].status = "Busy";
    ambulances[bestIdx].incidentId = incidentId;
    saveAll(ambulances);

    IncidentService::updateStatus(incidentId, "Assigned");

    res.success = true;
    res.ambulanceId = ambulances[bestIdx].id;
    res.fromLocation = ambulances[bestIdx].location;
    res.toLocation = targetNode;
    res.route = bestRoute.path;
    res.metres = bestRoute.totalMetres;
    res.etaMinutes = bestRoute.walkingMinutes;
    return res;
}

bool AmbulanceService::releaseAmbulance(const QString& ambulanceId)
{
    auto ambulances = loadAll();
    bool found = false;
    int linkedIncident = -1;

    for (auto& a : ambulances) {
        if (a.id == ambulanceId) {
            linkedIncident = a.incidentId;
            a.status = "Available";
            a.incidentId = -1;
            found = true;
            break;
        }
    }

    if (!found) return false;
    saveAll(ambulances);

    if (linkedIncident > 0) {
        IncidentService::updateStatus(linkedIncident, "Resolved");
    }
    return true;
}

