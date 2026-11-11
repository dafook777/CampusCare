#ifndef BADGE_H
#define BADGE_H

#include <QLabel>
#include <QStyle>

// A tiny coloured chip.  Set the "level" property to one of:
//   low, medium, high, critical, ok, warn, info, neutral
// and the QSS in StyleSheets.h colours it automatically.
class Badge : public QLabel {
    Q_OBJECT
    Q_PROPERTY(QString level READ level WRITE setLevel)
public:
    explicit Badge(const QString& text = {}, const QString& lvl = "neutral",
                   QWidget* parent = nullptr)
        : QLabel(text, parent)
    {
        setObjectName("badge");
        setAlignment(Qt::AlignCenter);
        setLevel(lvl);
    }

    QString level() const { return m_level; }

    void setLevel(const QString& l) {
        if (m_level == l) return;
        m_level = l;
        setProperty("level", l);
        style()->unpolish(this);
        style()->polish(this);
    }

private:
    QString m_level;
};

#endif // BADGE_H
