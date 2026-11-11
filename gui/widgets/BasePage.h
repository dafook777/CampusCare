#ifndef BASEPAGE_H
#define BASEPAGE_H

#include <QWidget>
#include <QString>

class BasePage : public QWidget {
    Q_OBJECT
public:
    explicit BasePage(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual ~BasePage() = default;

    virtual QString title() const = 0;
    virtual void    refresh() {}         // reload data from disk
};

#endif // BASEPAGE_H

