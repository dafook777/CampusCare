#include "StudentService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

QString StudentService::filePath()
{
    return DataPath::resolve("students.txt");
}

QVector<StudentItem> StudentService::loadAll()
{
    QVector<StudentItem> list;
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return list;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        const QStringList p = line.split('|');
        if (p.size() >= 7) {
            StudentItem s;
            s.studentId = p[0].trimmed();
            s.name = p[1].trimmed();
            s.department = p[2].trimmed();
            s.phone = p[3].trimmed();
            s.bloodGroup = p[4].trimmed();
            s.gender = p[5].trimmed();
            s.isActive = (p[6].trimmed() == "1");
            list.append(s);
        }
    }
    return list;
}

StudentItem StudentService::getById(const QString& id)
{
    auto list = loadAll();
    for (const auto& s : list)
        if (s.studentId == id) return s;
    return {};
}

bool StudentService::update(const StudentItem& s)
{
    auto list = loadAll();
    bool found = false;
    for (auto& existing : list) {
        if (existing.studentId == s.studentId) {
            existing = s;
            found = true;
            break;
        }
    }
    if (!found) list.append(s);

    QFile file(filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&file);
    for (const auto& st : list)
        out << st.studentId << "|" << st.name << "|" << st.department << "|"
            << st.phone << "|" << st.bloodGroup << "|" << st.gender << "|"
            << (st.isActive ? "1" : "0") << "|\n";
    return true;
}

