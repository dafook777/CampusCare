// ─────────────────────────────────────────────────────────────
// CampusCare GUI — Main Application Window
// ─────────────────────────────────────────────────────────────

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "AppContext.h"
#include "NavRegistry.h"

class QLabel;
class QPushButton;
class QStackedWidget;
class QButtonGroup;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const AppContext& ctx, QWidget* parent = nullptr);

signals:
    void logoutRequested();

private slots:
    void onNavClicked(int index);
    void onThemeToggle();
    void onLogout();
    void updateClock();

private:
    void buildSidebar();
    void buildTopBar();
    QWidget* getOrCreatePage(int index);

    AppContext            m_ctx;
    QWidget*              m_sidebar      = nullptr;
    QLabel*               m_pageTitle    = nullptr;
    QLabel*               m_statusBeacon = nullptr;
    QLabel*               m_clockLabel   = nullptr;
    QLabel*               m_userChip     = nullptr;
    QPushButton*          m_themeBtn     = nullptr;
    QStackedWidget*       m_stack        = nullptr;
    QButtonGroup*         m_navGroup     = nullptr;
    QVector<QPushButton*> m_navBtns;
    QVector<NavEntry>     m_nav;         // filtered for current role
    QVector<QWidget*>     m_pages;       // lazily created, same size as m_nav
    QTimer*               m_clockTimer   = nullptr;
};

#endif // MAINWINDOW_H
