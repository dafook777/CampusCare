#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>

class ThemeManager : public QObject {
    Q_OBJECT
public:
    enum Theme { Dark, Light };
    static ThemeManager& instance();
    void setTheme(Theme t);
    void toggleTheme();
    Theme currentTheme() const { return m_theme; }

signals:
    void themeChanged(Theme t);

private:
    ThemeManager() = default;
    Theme m_theme = Dark;
};

#endif // THEMEMANAGER_H
