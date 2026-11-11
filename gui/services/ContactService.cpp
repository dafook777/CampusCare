#include "ContactService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

QString ContactService::filePath()
{
    return DataPath::resolve("contacts.txt");
}

QVector<ContactItem> ContactService::loadAll()
{
    QVector<ContactItem> list;
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        const QStringList p = line.split('|');
        if (p.size() >= 3) {
            list.append({p[0].trimmed(), p[1].trimmed(), p[2].trimmed()});
        }
    }
    return list;
}

static void saveAll(const QVector<ContactItem>& list)
{
    QFile file(ContactService::filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return;
    QTextStream out(&file);
    for (const auto& c : list)
        out << c.name << "|" << c.number << "|" << c.category << "\n";
}

bool ContactService::addContact(const ContactItem& c)
{
    auto list = loadAll();
    list.append(c);
    saveAll(list);
    return true;
}

bool ContactService::deleteContact(const QString& name)
{
    auto list = loadAll();
    list.erase(std::remove_if(list.begin(), list.end(),
        [&](const ContactItem& c){ return c.name == name; }), list.end());
    saveAll(list);
    return true;
}

