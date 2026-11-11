#ifndef PLACEHOLDERPAGE_H
#define PLACEHOLDERPAGE_H

#include "BasePage.h"
#include <QStringList>

class PlaceholderPage : public BasePage {
    Q_OBJECT
public:
    explicit PlaceholderPage(const QString& title, const QStringList& todo, QWidget* parent = nullptr);

    QString title() const override { return m_title; }
    void refresh() override {}

private:
    QString m_title;
};

#endif // PLACEHOLDERPAGE_H

