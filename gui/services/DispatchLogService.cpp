#include "DispatchLogService.h"
#include "DataPath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

QString DispatchLogService::filePath()
{
    return DataPath::resolve("dispatch_log.txt");
}

QStringList DispatchLogService::loadAll()
{
    QStringList lines;
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (!line.isEmpty()) lines.append(line);
    }
    return lines;
}

