#include "IncidentService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <algorithm>

QString IncidentService::incidentsFilePath()
{
    return DataPath::resolve("incidents.txt");
}

QVector<IncidentItem> IncidentService::loadAll()
{
    QVector<IncidentItem> list;
    QFile file(incidentsFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;

    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        const QStringList parts = line.split('|');
        if (parts.size() >= 8) {
            IncidentItem item;
            item.id = parts[0].toInt();
            item.reporter = parts[1];
            item.location = parts[2];
            item.type = parts[3];
            item.severity = parts[4].toInt();
            item.description = parts[5];
            item.ownerId = parts[6];
            item.status = parts[7];
            list.append(item);
        }
    }
    return list;
}

QVector<IncidentItem> IncidentService::loadPriorityQueue()
{
    auto list = loadAll();
    std::sort(list.begin(), list.end(), [](const IncidentItem& a, const IncidentItem& b) {
        if (a.severity != b.severity) return a.severity > b.severity; // higher severity first
        return a.id < b.id;
    });
    return list;
}

static bool saveAll(const QVector<IncidentItem>& list)
{
    QFile file(IncidentService::incidentsFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;

    QTextStream out(&file);
    for (const auto& item : list) {
        out << item.id << '|'
            << item.reporter << '|'
            << item.location << '|'
            << item.type << '|'
            << item.severity << '|'
            << item.description << '|'
            << item.ownerId << '|'
            << item.status << '\n';
    }
    return true;
}

bool IncidentService::addIncident(const IncidentItem& item)
{
    const auto all = loadAll();
    int nextId = 1001;
    for (const auto& i : all) {
        if (i.id >= nextId) nextId = i.id + 1;
    }

    auto updated = all;
    IncidentItem toAdd = item;
    toAdd.id = nextId;
    if (toAdd.status.isEmpty()) toAdd.status = "Open";
    updated.append(toAdd);
    return saveAll(updated);
}

bool IncidentService::updateStatus(int id, const QString& newStatus)
{
    auto all = loadAll();
    bool found = false;
    for (auto& i : all) {
        if (i.id == id) {
            i.status = newStatus;
            found = true;
            break;
        }
    }
    if (!found) return false;
    return saveAll(all);
}

bool IncidentService::deleteIncident(int id)
{
    auto all = loadAll();
    int idx = -1;
    for (int i = 0; i < all.size(); ++i) {
        if (all[i].id == id) { idx = i; break; }
    }
    if (idx == -1) return false;
    all.removeAt(idx);
    return saveAll(all);
}

IncidentItem IncidentService::getById(int id)
{
    const auto all = loadAll();
    for (const auto& i : all) {
        if (i.id == id) return i;
    }
    return IncidentItem{};
}
