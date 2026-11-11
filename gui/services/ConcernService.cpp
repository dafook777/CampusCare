#include "ConcernService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <algorithm>

QString ConcernService::filePath()
{
    return DataPath::resolve("concerns.txt");
}

QVector<ConcernItem> ConcernService::loadAll()
{
    QVector<ConcernItem> list;
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        const QStringList p = line.split('|');
        if (p.size() >= 8) {
            ConcernItem c;
            c.id = p[0].trimmed().toInt();
            c.studentId = p[1].trimmed();
            c.studentName = p[2].trimmed();
            c.facultyId = p[3].trimmed();
            c.category = p[4].trimmed();
            c.description = p[5].trimmed();
            c.isAnonymous = (p[6].trimmed() == "1");
            c.status = p[7].trimmed();
            list.append(c);
        }
    }
    return list;
}

static void saveAll(const QVector<ConcernItem>& list)
{
    QFile file(ConcernService::filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return;
    QTextStream out(&file);
    for (const auto& c : list)
        out << c.id << "|" << c.studentId << "|" << c.studentName << "|"
            << c.facultyId << "|" << c.category << "|" << c.description << "|"
            << (c.isAnonymous ? "1" : "0") << "|" << c.status << "\n";
}

bool ConcernService::addConcern(ConcernItem& item)
{
    auto list = loadAll();
    int maxId = 2000;
    for (const auto& c : list) if (c.id >= maxId) maxId = c.id + 1;
    item.id = maxId;
    list.append(item);
    saveAll(list);
    return true;
}

bool ConcernService::updateStatus(int id, const QString& newStatus)
{
    auto list = loadAll();
    for (auto& c : list) {
        if (c.id == id) { c.status = newStatus; break; }
    }
    saveAll(list);
    return true;
}

bool ConcernService::deleteConcern(int id)
{
    auto list = loadAll();
    list.erase(std::remove_if(list.begin(), list.end(),
        [&](const ConcernItem& c){ return c.id == id; }), list.end());
    saveAll(list);
    return true;
}

