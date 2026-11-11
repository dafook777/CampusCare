#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QString>

struct AppContext {
    QString userId;
    QString role;        // "admin", "student", "faculty"
    QString projectRoot; // directory containing users.txt / incidents.txt etc.
};

#endif // APPCONTEXT_H

