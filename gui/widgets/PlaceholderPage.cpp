#include "PlaceholderPage.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

PlaceholderPage::PlaceholderPage(const QString& title, const QStringList& todo, QWidget* parent)
    : BasePage(parent), m_title(title)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(32, 32, 32, 32);
    outer->setAlignment(Qt::AlignCenter);

    auto* card = new QFrame;
    card->setObjectName("card");
    card->setFixedWidth(560);

    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(36, 36, 36, 36);
    layout->setSpacing(14);
    layout->setAlignment(Qt::AlignCenter);

    auto* icon = new QLabel(QString::fromUtf8("\xF0\x9F\x9A\xA7"));
    icon->setObjectName("placeholderIcon");
    icon->setAlignment(Qt::AlignCenter);
    layout->addWidget(icon);

    auto* t = new QLabel(title);
    t->setObjectName("placeholderTitle");
    t->setAlignment(Qt::AlignCenter);
    layout->addWidget(t);

    QString text = "<b>Implementation Roadmap:</b><br><br>";
    for (const auto& item : todo) {
        text += QString::fromUtf8("• ") + item + "<br>";
    }

    auto* list = new QLabel(text);
    list->setObjectName("placeholderList");
    list->setWordWrap(true);
    list->setAlignment(Qt::AlignLeft);
    layout->addWidget(list);

    outer->addWidget(card);
}

