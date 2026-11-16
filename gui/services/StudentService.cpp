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

QString StudentService::photoPath(const QString& studentId)
{
    if (studentId.isEmpty()) return QString();

    const QStringList extensions = {".jpg", ".png", ".jpeg"};
    const QStringList prefixes = {
        "photos/",
        "data/photos/",
        "../photos/",
        "../../photos/"
    };

    for (const auto& ext : extensions) {
        for (const auto& prefix : prefixes) {
            const QString candidate = DataPath::resolve(prefix + studentId + ext);
            if (QFile::exists(candidate)) {
                return candidate;
            }
        }
    }
    return QString();
}

bool StudentService::savePhoto(const QString& studentId, const QString& sourceImagePath)
{
    if (studentId.isEmpty() || sourceImagePath.isEmpty()) return false;
    if (!QFile::exists(sourceImagePath)) return false;

    QDir base(QCoreApplication::applicationDirPath());
    base.mkpath("photos");
    if (base.exists("data")) {
        base.mkpath("data/photos");
    }

    const QString dest1 = base.filePath("photos/" + studentId + ".jpg");
    if (QFile::exists(dest1)) QFile::remove(dest1);
    bool ok = QFile::copy(sourceImagePath, dest1);

    if (base.exists("data")) {
        const QString dest2 = base.filePath("data/photos/" + studentId + ".jpg");
        if (QFile::exists(dest2)) QFile::remove(dest2);
        QFile::copy(sourceImagePath, dest2);
    }
    return ok;
}

