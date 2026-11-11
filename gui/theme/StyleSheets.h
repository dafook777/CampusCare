#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QString>

namespace StyleSheets {

inline QString dark()
{
    return QStringLiteral(
        /* ── Global ────────────────────────────────────── */
        "QMainWindow, QWidget#centralContainer { background: #12171f; }"
        "QToolBar     { background: #1a212c; border: none; border-bottom: 1px solid #2a3444; }"
        "QStatusBar   { background: #1a212c; color: #8b97a7; }"

        /* ── Old branded toolbar labels (kept for compat) */
        "QLabel#brandLabel    { color: #e8edf3; font-size: 18px; font-weight: 600; }"
        "QLabel#themeLabel    { color: #8b97a7; font-size: 12px; }"
        "QPushButton#themeToggleBtn { background: transparent; border: 1px solid #8b97a7;"
        "  border-radius: 18px; font-size: 16px; color: #e8edf3; }"
        "QPushButton#themeToggleBtn:hover { background: #03a9f4; }"
        "QLabel#welcomeTitle    { color: #e8edf3; font-size: 36px; font-weight: 700; }"
        "QLabel#welcomeSubtitle { color: #8b97a7; font-size: 16px; }"
        "QLabel#statusLabel     { color: #03a9f4; font-size: 14px; }"
        "QLabel#versionLabel    { color: #8b97a7; font-size: 12px; }"

        /* ── Sidebar ───────────────────────────────────── */
        "QWidget#sidebar {"
        "  background: #0e1319;"
        "}"
        "QLabel#sidebarBrand {"
        "  color: #e8edf3; font-size: 20px; font-weight: 700;"
        "  padding: 24px 16px 8px 16px; background: transparent;"
        "}"
        "QLabel#sidebarBrandSub {"
        "  color: #5a6a7a; font-size: 11px;"
        "  padding: 0 16px 16px 16px; background: transparent;"
        "}"

        /* ── Nav buttons ───────────────────────────────── */
        "QPushButton#navBtn {"
        "  background: transparent; color: #8b97a7;"
        "  border: none; border-left: 3px solid transparent;"
        "  text-align: left; padding: 10px 18px; font-size: 14px;"
        "  border-radius: 0;"
        "}"
        "QPushButton#navBtn:hover {"
        "  background: #1a212c; color: #c8d0dc;"
        "}"
        "QPushButton#navBtn:checked {"
        "  background: #1a212c; color: #03a9f4;"
        "  border-left: 3px solid #03a9f4;"
        "  font-weight: 600;"
        "}"

        /* ── User card in sidebar ──────────────────────── */
        "QFrame#userCard {"
        "  background: #161d28; border: 1px solid #2a3444; border-radius: 8px;"
        "  margin: 8px 12px;"
        "}"
        "QLabel#userCardId   { color: #e8edf3; font-size: 13px; font-weight: 600; background: transparent; }"
        "QLabel#userCardRole { color: #5a6a7a; font-size: 11px; background: transparent; }"
        "QPushButton#logoutBtn {"
        "  background: transparent; color: #e57373; border: 1px solid #e57373;"
        "  border-radius: 8px; font-size: 13px; padding: 6px 12px;"
        "  margin: 4px 12px 12px 12px;"
        "}"
        "QPushButton#logoutBtn:hover { background: #2a1a1a; }"

        /* ── Top bar ───────────────────────────────────── */
        "QWidget#topBar {"
        "  background: #1a212c; border-bottom: 1px solid #2a3444;"
        "}"
        "QLabel#pageTitle {"
        "  color: #e8edf3; font-size: 22px; font-weight: 700;"
        "  padding-left: 4px; background: transparent;"
        "}"
        "QLabel#userChip {"
        "  color: #8b97a7; background: #12171f; border: 1px solid #2a3444;"
        "  border-radius: 12px; padding: 4px 14px; font-size: 12px;"
        "}"

        /* ── DataTable / QTableWidget ──────────────────── */
        "QLineEdit#searchBox {"
        "  background: #1a212c; color: #e8edf3; border: 1px solid #2a3444;"
        "  border-radius: 8px; padding: 8px 14px; font-size: 14px;"
        "}"
        "QLineEdit#searchBox:focus { border: 1px solid #03a9f4; }"
        "QTableWidget#dataTable {"
        "  background: #12171f; alternate-background-color: #161d28;"
        "  color: #e8edf3; gridline-color: #2a3444;"
        "  border: 1px solid #2a3444; border-radius: 8px;"
        "  font-size: 13px; selection-background-color: #1e3a5f;"
        "}"
        "QTableWidget#dataTable QHeaderView::section {"
        "  background: #1a212c; color: #8b97a7; border: none;"
        "  border-bottom: 2px solid #2a3444; padding: 8px 12px;"
        "  font-weight: 600; font-size: 13px;"
        "}"
        "QTableWidget#dataTable::item { padding: 6px 12px; }"
        "QScrollBar:vertical { background: #12171f; width: 8px; }"
        "QScrollBar::handle:vertical { background: #2a3444; border-radius: 4px; min-height: 30px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"

        /* ── Badges ────────────────────────────────────── */
        "QLabel#badge {"
        "  font-size: 12px; font-weight: 600; border-radius: 8px;"
        "  padding: 3px 10px; min-width: 50px;"
        "}"
        "QLabel#badge[level=\"low\"]      { background: #1b3a2a; color: #66bb6a; }"
        "QLabel#badge[level=\"medium\"]   { background: #3a3520; color: #ffa726; }"
        "QLabel#badge[level=\"high\"]     { background: #3a2020; color: #ef5350; }"
        "QLabel#badge[level=\"critical\"] { background: #4a1020; color: #ff1744; }"
        "QLabel#badge[level=\"ok\"]       { background: #1b3a2a; color: #66bb6a; }"
        "QLabel#badge[level=\"warn\"]     { background: #3a3520; color: #ffa726; }"
        "QLabel#badge[level=\"info\"]     { background: #1a2a3a; color: #42a5f5; }"
        "QLabel#badge[level=\"neutral\"] { background: #1a212c; color: #8b97a7; }"

        /* ── Placeholder page ──────────────────────────── */
        "QLabel#placeholderIcon  { font-size: 48px; color: #2a3444; background: transparent; }"
        "QLabel#placeholderTitle { font-size: 22px; font-weight: 700; color: #5a6a7a; background: transparent; }"
        "QLabel#placeholderList  { font-size: 14px; color: #5a6a7a; line-height: 180%; background: transparent; }"

        /* ── General inputs & buttons (scoped to content area) ── */
        "QWidget#contentArea QLineEdit {"
        "  background: #1a212c; color: #e8edf3; border: 1px solid #2a3444;"
        "  border-radius: 8px; padding: 8px 14px; font-size: 14px;"
        "}"
        "QWidget#contentArea QLineEdit:focus { border: 1px solid #03a9f4; }"
        "QWidget#contentArea QPushButton {"
        "  background: #03a9f4; color: white; border: none; border-radius: 8px;"
        "  padding: 8px 20px; font-size: 14px; font-weight: 600;"
        "}"
        "QWidget#contentArea QPushButton:hover { background: #29b6f6; }"
        "QWidget#contentArea QPushButton:pressed { background: #0288d1; }"
        "QDialog QPushButton {"
        "  background: #03a9f4; color: white; border: none; border-radius: 8px;"
        "  padding: 8px 20px; font-size: 14px; font-weight: 600;"
        "}"
        "QDialog QPushButton:hover { background: #29b6f6; }"
        "QDialog QPushButton:pressed { background: #0288d1; }"
        "QDialog QLineEdit {"
        "  background: #1a212c; color: #e8edf3; border: 1px solid #2a3444;"
        "  border-radius: 8px; padding: 8px 14px; font-size: 14px;"
        "}"
        "QDialog QLineEdit:focus { border: 1px solid #03a9f4; }"

        /* ── Card container ────────────────────────────── */
        "QFrame#card {"
        "  background: #1a212c; border: 1px solid #2a3444; border-radius: 8px;"
        "}"
    );
}

inline QString light()
{
    return QStringLiteral(
        /* ── Global ────────────────────────────────────── */
        "QMainWindow, QWidget#centralContainer { background: #f3f6f9; }"
        "QToolBar     { background: #ffffff; border: none; border-bottom: 1px solid #e0e4ea; }"
        "QStatusBar   { background: #ffffff; color: #5f6b7a; }"

        /* ── Old branded toolbar labels (kept for compat) */
        "QLabel#brandLabel    { color: #1d2733; font-size: 18px; font-weight: 600; }"
        "QLabel#themeLabel    { color: #5f6b7a; font-size: 12px; }"
        "QPushButton#themeToggleBtn { background: transparent; border: 1px solid #5f6b7a;"
        "  border-radius: 18px; font-size: 16px; color: #1d2733; }"
        "QPushButton#themeToggleBtn:hover { background: #0288d1; color: white; }"
        "QLabel#welcomeTitle    { color: #1d2733; font-size: 36px; font-weight: 700; }"
        "QLabel#welcomeSubtitle { color: #5f6b7a; font-size: 16px; }"
        "QLabel#statusLabel     { color: #0288d1; font-size: 14px; }"
        "QLabel#versionLabel    { color: #5f6b7a; font-size: 12px; }"

        /* ── Sidebar ───────────────────────────────────── */
        "QWidget#sidebar {"
        "  background: #1a212c;"
        "}"
        "QLabel#sidebarBrand {"
        "  color: #e8edf3; font-size: 20px; font-weight: 700;"
        "  padding: 24px 16px 8px 16px; background: transparent;"
        "}"
        "QLabel#sidebarBrandSub {"
        "  color: #5a6a7a; font-size: 11px;"
        "  padding: 0 16px 16px 16px; background: transparent;"
        "}"

        /* ── Nav buttons ───────────────────────────────── */
        "QPushButton#navBtn {"
        "  background: transparent; color: #8b97a7;"
        "  border: none; border-left: 3px solid transparent;"
        "  text-align: left; padding: 10px 18px; font-size: 14px;"
        "  border-radius: 0;"
        "}"
        "QPushButton#navBtn:hover {"
        "  background: #242d3a; color: #c8d0dc;"
        "}"
        "QPushButton#navBtn:checked {"
        "  background: #242d3a; color: #29b6f6;"
        "  border-left: 3px solid #0288d1;"
        "  font-weight: 600;"
        "}"

        /* ── User card in sidebar ──────────────────────── */
        "QFrame#userCard {"
        "  background: #242d3a; border: 1px solid #2a3444; border-radius: 8px;"
        "  margin: 8px 12px;"
        "}"
        "QLabel#userCardId   { color: #e8edf3; font-size: 13px; font-weight: 600; background: transparent; }"
        "QLabel#userCardRole { color: #5a6a7a; font-size: 11px; background: transparent; }"
        "QPushButton#logoutBtn {"
        "  background: transparent; color: #e57373; border: 1px solid #e57373;"
        "  border-radius: 8px; font-size: 13px; padding: 6px 12px;"
        "  margin: 4px 12px 12px 12px;"
        "}"
        "QPushButton#logoutBtn:hover { background: #3a1a1a; }"

        /* ── Top bar ───────────────────────────────────── */
        "QWidget#topBar {"
        "  background: #ffffff; border-bottom: 1px solid #e0e4ea;"
        "}"
        "QLabel#pageTitle {"
        "  color: #1d2733; font-size: 22px; font-weight: 700;"
        "  padding-left: 4px; background: transparent;"
        "}"
        "QLabel#userChip {"
        "  color: #5f6b7a; background: #f3f6f9; border: 1px solid #e0e4ea;"
        "  border-radius: 12px; padding: 4px 14px; font-size: 12px;"
        "}"

        /* ── DataTable / QTableWidget ──────────────────── */
        "QLineEdit#searchBox {"
        "  background: #ffffff; color: #1d2733; border: 1px solid #e0e4ea;"
        "  border-radius: 8px; padding: 8px 14px; font-size: 14px;"
        "}"
        "QLineEdit#searchBox:focus { border: 1px solid #0288d1; }"
        "QTableWidget#dataTable {"
        "  background: #ffffff; alternate-background-color: #f8f9fb;"
        "  color: #1d2733; gridline-color: #e0e4ea;"
        "  border: 1px solid #e0e4ea; border-radius: 8px;"
        "  font-size: 13px; selection-background-color: #d6ecf8;"
        "}"
        "QTableWidget#dataTable QHeaderView::section {"
        "  background: #f3f6f9; color: #5f6b7a; border: none;"
        "  border-bottom: 2px solid #e0e4ea; padding: 8px 12px;"
        "  font-weight: 600; font-size: 13px;"
        "}"
        "QTableWidget#dataTable::item { padding: 6px 12px; }"
        "QScrollBar:vertical { background: #f3f6f9; width: 8px; }"
        "QScrollBar::handle:vertical { background: #c0c8d4; border-radius: 4px; min-height: 30px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"

        /* ── Badges ────────────────────────────────────── */
        "QLabel#badge {"
        "  font-size: 12px; font-weight: 600; border-radius: 8px;"
        "  padding: 3px 10px; min-width: 50px;"
        "}"
        "QLabel#badge[level=\"low\"]      { background: #e8f5e9; color: #2e7d32; }"
        "QLabel#badge[level=\"medium\"]   { background: #fff3e0; color: #e65100; }"
        "QLabel#badge[level=\"high\"]     { background: #ffebee; color: #c62828; }"
        "QLabel#badge[level=\"critical\"] { background: #fce4ec; color: #b71c1c; }"
        "QLabel#badge[level=\"ok\"]       { background: #e8f5e9; color: #2e7d32; }"
        "QLabel#badge[level=\"warn\"]     { background: #fff3e0; color: #e65100; }"
        "QLabel#badge[level=\"info\"]     { background: #e3f2fd; color: #1565c0; }"
        "QLabel#badge[level=\"neutral\"] { background: #eceff1; color: #607d8b; }"

        /* ── Placeholder page ──────────────────────────── */
        "QLabel#placeholderIcon  { font-size: 48px; color: #c0c8d4; background: transparent; }"
        "QLabel#placeholderTitle { font-size: 22px; font-weight: 700; color: #5f6b7a; background: transparent; }"
        "QLabel#placeholderList  { font-size: 14px; color: #5f6b7a; line-height: 180%; background: transparent; }"

        /* ── General inputs & buttons (scoped to content area) ── */
        "QWidget#contentArea QLineEdit {"
        "  background: #ffffff; color: #1d2733; border: 1px solid #e0e4ea;"
        "  border-radius: 8px; padding: 8px 14px; font-size: 14px;"
        "}"
        "QWidget#contentArea QLineEdit:focus { border: 1px solid #0288d1; }"
        "QWidget#contentArea QPushButton {"
        "  background: #0288d1; color: white; border: none; border-radius: 8px;"
        "  padding: 8px 20px; font-size: 14px; font-weight: 600;"
        "}"
        "QWidget#contentArea QPushButton:hover { background: #29b6f6; }"
        "QWidget#contentArea QPushButton:pressed { background: #01579b; }"
        "QDialog QPushButton {"
        "  background: #0288d1; color: white; border: none; border-radius: 8px;"
        "  padding: 8px 20px; font-size: 14px; font-weight: 600;"
        "}"
        "QDialog QPushButton:hover { background: #29b6f6; }"
        "QDialog QPushButton:pressed { background: #01579b; }"
        "QDialog QLineEdit {"
        "  background: #ffffff; color: #1d2733; border: 1px solid #e0e4ea;"
        "  border-radius: 8px; padding: 8px 14px; font-size: 14px;"
        "}"
        "QDialog QLineEdit:focus { border: 1px solid #0288d1; }"

        /* ── Card container ────────────────────────────── */
        "QFrame#card {"
        "  background: #ffffff; border: 1px solid #e0e4ea; border-radius: 8px;"
        "}"
    );
}

} // namespace StyleSheets

#endif // STYLESHEETS_H
