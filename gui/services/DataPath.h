#ifndef DATAPATH_H
#define DATAPATH_H

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QString>

namespace DataPath {
inline QString resolve(const QString& filename)
{
    const QDir base(QCoreApplication::applicationDirPath());

    // 1. Look in "data" folder next to the exe first
    const QString inData = base.filePath("data/" + filename);
    if (QFile::exists(inData)) return inData;

    // 2. Fall back to current paths / parent paths
    for (const char* rel : {"", "../", "../../", "../../../"}) {
        const QString p = base.filePath(QString(rel) + filename);
        if (QFile::exists(p)) return p;
    }

    // If data directory exists next to exe, return data path
    if (base.exists("data")) return inData;

    return filename;
}
}

#endif // DATAPATH_H

