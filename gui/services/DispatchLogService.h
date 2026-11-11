#ifndef DISPATCHLOGSERVICE_H
#define DISPATCHLOGSERVICE_H
#include <QString>
#include <QStringList>
class DispatchLogService {
public:
    static QString filePath();
    static QStringList loadAll();
};
#endif

