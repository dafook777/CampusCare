#include "NotificationsPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/NotificationService.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

NotificationsPage::NotificationsPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    auto* topRow = new QHBoxLayout;
    m_unreadLbl = new QLabel(this);
    m_unreadLbl->setStyleSheet("font-size: 14px; font-weight: 600;");
    topRow->addWidget(m_unreadLbl);
    topRow->addStretch(1);

    auto* readBtn = new QPushButton("Mark Selected Read", this);
    readBtn->setCursor(Qt::PointingHandCursor);
    readBtn->setFixedHeight(36);
    connect(readBtn, &QPushButton::clicked, this, &NotificationsPage::onMarkRead);
    topRow->addWidget(readBtn);

    layout->addLayout(topRow);
    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);
    refresh();
}

void NotificationsPage::refresh()
{
    const auto notifs = (m_ctx.role == "admin")
        ? NotificationService::loadAll()
        : NotificationService::loadForUser(m_ctx.userId);

    int unread = 0;
    for (const auto& n : notifs) if (!n.isRead) ++unread;
    m_unreadLbl->setText(QString("Unread: %1 / Total: %2").arg(unread).arg(notifs.size()));

    QStringList headers = {"ID", "Recipient", "Incident", "Type", "Message", "Status"};
    QVector<QStringList> rows;
    for (const auto& n : notifs) {
        rows.append({
            QString::number(n.notificationId),
            n.recipientId,
            n.incidentId > 0 ? QString::number(n.incidentId) : "-",
            n.notificationType,
            n.message,
            n.isRead ? "Read" : "Unread"
        });
    }
    m_table->setData(headers, rows);

    for (int i = 0; i < notifs.size(); ++i) {
        QString typeLvl = "info";
        if (notifs[i].notificationType.contains("Critical", Qt::CaseInsensitive)) typeLvl = "critical";
        else if (notifs[i].notificationType.contains("Dispatch", Qt::CaseInsensitive)) typeLvl = "warn";
        m_table->setCellWidget(i, 3, new Badge(notifs[i].notificationType, typeLvl));

        QString statusLvl = notifs[i].isRead ? "neutral" : "high";
        m_table->setCellWidget(i, 5, new Badge(notifs[i].isRead ? "Read" : "Unread", statusLvl));
    }
}

void NotificationsPage::onMarkRead()
{
    auto* table = m_table->table();
    int r = table->currentRow();
    if (r < 0) { QMessageBox::information(this, "Select", "Select a notification row first."); return; }
    auto* item = table->item(r, 0);
    if (!item) return;
    NotificationService::markRead(item->text().toInt());
    refresh();
}

