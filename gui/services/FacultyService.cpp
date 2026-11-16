#include "FacultyService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

QString FacultyService::filePath()
{
    return DataPath::resolve("faculty_profiles.txt");
}

QVector<FacultyItem> FacultyService::loadAll()
{
    QVector<FacultyItem> list;
    QFile file(filePath());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            const QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            const QStringList p = line.split('|');
            if (p.size() >= 7) {
                FacultyItem f;
                f.facultyId = p[0].trimmed();
                f.name = p[1].trimmed();
                f.department = p[2].trimmed();
                f.phone = p[3].trimmed();
                f.bloodGroup = p[4].trimmed();
                f.gender = p[5].trimmed();
                f.isActive = (p[6].trimmed() == "1");
                if (p.size() >= 8 && !p[7].trimmed().isEmpty()) {
                    f.designation = p[7].trimmed();
                }
                list.append(f);
            }
        }
    }

    // Default fallback if empty
    if (list.isEmpty()) {
        FacultyItem def;
        def.facultyId = "pleaseletmelogin";
        def.name = "Dr. Neeraj Kumar";
        def.department = "Computer Science & Engineering";
        def.phone = "9876543210";
        def.bloodGroup = "O+";
        def.gender = "Male";
        def.isActive = true;
        def.designation = "Associate Professor";
        list.append(def);
    }

    return list;
}

FacultyItem FacultyService::getById(const QString& id)
{
    auto list = loadAll();
    for (const auto& f : list) {
        if (f.facultyId.compare(id, Qt::CaseInsensitive) == 0) return f;
    }

    // Fallback stub for unknown faculty ID
    FacultyItem stub;
    stub.facultyId = id;
    stub.name = "Faculty Member " + id;
    stub.department = "Academic Faculty";
    stub.phone = "Not registered";
    stub.bloodGroup = "Unknown";
    stub.gender = "Unspecified";
    stub.isActive = true;
    stub.designation = "Faculty Member";
    return stub;
}

bool FacultyService::update(const FacultyItem& f)
{
    auto list = loadAll();
    bool found = false;
    for (auto& existing : list) {
        if (existing.facultyId.compare(f.facultyId, Qt::CaseInsensitive) == 0) {
            existing = f;
            found = true;
            break;
        }
    }
    if (!found) list.append(f);

    QFile file(filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&file);
    for (const auto& item : list) {
        out << item.facultyId << "|" << item.name << "|" << item.department << "|"
            << item.phone << "|" << item.bloodGroup << "|" << item.gender << "|"
            << (item.isActive ? "1" : "0") << "|" << item.designation << "|\n";
    }
    return true;
}

QString FacultyService::photoPath(const QString& facultyId)
{
    if (facultyId.isEmpty()) return QString();

    const QStringList extensions = {".jpg", ".png", ".jpeg"};
    const QStringList prefixes = {
        "photos/",
        "data/photos/",
        "../photos/",
        "../../photos/"
    };

    for (const auto& ext : extensions) {
        for (const auto& prefix : prefixes) {
            const QString candidate = DataPath::resolve(prefix + facultyId + ext);
            if (QFile::exists(candidate)) {
                return candidate;
            }
        }
    }
    return QString();
}

bool FacultyService::savePhoto(const QString& facultyId, const QString& sourceImagePath)
{
    if (facultyId.isEmpty() || sourceImagePath.isEmpty()) return false;
    if (!QFile::exists(sourceImagePath)) return false;

    QDir base(QCoreApplication::applicationDirPath());
    base.mkpath("photos");
    if (base.exists("data")) {
        base.mkpath("data/photos");
    }

    const QString dest1 = base.filePath("photos/" + facultyId + ".jpg");
    if (QFile::exists(dest1)) QFile::remove(dest1);
    bool ok = QFile::copy(sourceImagePath, dest1);

    if (base.exists("data")) {
        const QString dest2 = base.filePath("data/photos/" + facultyId + ".jpg");
        if (QFile::exists(dest2)) QFile::remove(dest2);
        QFile::copy(sourceImagePath, dest2);
    }
    return ok;
}
