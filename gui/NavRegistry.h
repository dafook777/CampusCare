#ifndef NAVREGISTRY_H
#define NAVREGISTRY_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <functional>

class QWidget;
struct AppContext;

struct NavEntry {
    QString       id;
    QString       label;
    QStringList   roles;       // which roles see this item
    // factory: creates the page widget; receives parent + context
    std::function<QWidget*(QWidget*, const AppContext&)> factory;
};

// Returns the master nav list.  Add new pages here.
QVector<NavEntry> buildNavRegistry();

#endif // NAVREGISTRY_H

