#include <QApplication>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QPointer>
#include <QTextStream>
#include <windows.h>
#include "AppContext.h"
#include "LoginWindow.h"
#include "MainWindow.h"
#include "theme/ThemeManager.h"

static void logMessage(const QString& msg)
{
    QFile f("gui_log.txt");
    if (f.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&f);
        out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "] " << msg << "\n";
    }
}

static void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString prefix;
    switch (type) {
        case QtDebugMsg:    prefix = "DEBUG: "; break;
        case QtInfoMsg:     prefix = "INFO: "; break;
        case QtWarningMsg:  prefix = "WARNING: "; break;
        case QtCriticalMsg: prefix = "CRITICAL: "; break;
        case QtFatalMsg:    prefix = "FATAL: "; break;
    }
    logMessage(prefix + msg + QString(" (%1:%2)").arg(context.file ? context.file : "").arg(context.line));
}

static LONG WINAPI unhandledCrashHandler(EXCEPTION_POINTERS* ep)
{
    const DWORD code = ep && ep->ExceptionRecord ? ep->ExceptionRecord->ExceptionCode : 0;
    const void* addr = ep && ep->ExceptionRecord ? ep->ExceptionRecord->ExceptionAddress : nullptr;
    logMessage(QString("CRASH: Exception 0x%1 at address %2").arg(code, 8, 16, QChar('0')).arg(reinterpret_cast<quintptr>(addr), 0, 16));
    return EXCEPTION_CONTINUE_SEARCH;
}

static QString findAndSetProjectRoot()
{
    QDir dir(QCoreApplication::applicationDirPath());
    if (dir.exists("data/users.txt")) {
        const QString dataDir = dir.filePath("data");
        QDir::setCurrent(dataDir);
        return dataDir;
    }
    for (int i = 0; i < 6; ++i) {
        if (dir.exists("users.txt")) {
            const QString root = dir.canonicalPath();
            QDir::setCurrent(root);
            return root;
        }
        if (!dir.cdUp()) break;
    }
    return QDir::currentPath();
}

int main(int argc, char* argv[])
{
    SetUnhandledExceptionFilter(unhandledCrashHandler);
    qInstallMessageHandler(customMessageHandler);

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    app.setApplicationName("CampusCare");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("GEHU");

    const QString projectRoot = findAndSetProjectRoot();
    logMessage("App started. Project root: " + projectRoot);

    ThemeManager::instance().setTheme(ThemeManager::Dark);

    auto* login = new LoginWindow;

    QObject::connect(login, &LoginWindow::loggedIn, [login, projectRoot, &app](const QString& id, const QString& role) {
        logMessage(QString("Logged in: id=%1, role=%2").arg(id, role));

        AppContext ctx;
        ctx.userId = id;
        ctx.role = role;
        ctx.projectRoot = projectRoot;

        auto* mw = new MainWindow(ctx);

        QObject::connect(mw, &MainWindow::logoutRequested, [login, mw]() {
            logMessage("Logout requested.");
            mw->hide();
            mw->deleteLater();
            login->show();
        });

        // When MainWindow is closed by clicking 'X', exit the application cleanly
        QObject::connect(mw, &QObject::destroyed, [&app, login]() {
            if (!login->isVisible()) {
                logMessage("Main window destroyed, quitting app.");
                app.quit();
            }
        });

        mw->show();
        login->hide();
    });

    login->show();

    const int ret = app.exec();
    logMessage(QString("App exiting with code %1").arg(ret));
    return ret;
}
