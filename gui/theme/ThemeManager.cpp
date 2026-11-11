#include "ThemeManager.h"
#include "StyleSheets.h"
#include <QApplication>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager m;
    return m;
}

void ThemeManager::setTheme(Theme t)
{
    m_theme = t;
    qApp->setStyleSheet(t == Dark ? StyleSheets::dark() : StyleSheets::light());
    emit themeChanged(t);
}

void ThemeManager::toggleTheme()
{
    setTheme(m_theme == Dark ? Light : Dark);
}
