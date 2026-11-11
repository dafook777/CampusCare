#include "AmbulancePage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/AmbulanceService.h"
#include "../services/IncidentService.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

AmbulancePage::AmbulancePage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    // ── Dispatch Status Card ─────────────────────────────────
    auto* summaryCard = new QFrame(this);
    summaryCard->setObjectName("card");
    auto* cardLayout = new QHBoxLayout(summaryCard);
    cardLayout->setContentsMargins(18, 14, 18, 14);

    m_dispatchSummary = new QLabel("Fleet Status: Ready for emergency dispatch.", summaryCard);
    m_dispatchSummary->setStyleSheet("font-size: 13px; font-weight: 600; color: #8b97a7;");
    cardLayout->addWidget(m_dispatchSummary, 1);

    const bool canManage = (m_ctx.role == "admin" || m_ctx.role == "faculty");
    if (canManage) {
        m_releaseBtn = new QPushButton(QString::fromUtf8("\xE2\x9C\x93 Release / Complete Mission"), summaryCard);
        m_releaseBtn->setCursor(Qt::PointingHandCursor);
        m_releaseBtn->setFixedHeight(36);
        m_releaseBtn->setStyleSheet("background: #2e7d32; color: white; font-weight: 600;");
        connect(m_releaseBtn, &QPushButton::clicked, this, &AmbulancePage::onReleaseAmbulance);
        cardLayout->addWidget(m_releaseBtn);
    }

    layout->addWidget(summaryCard);

    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);

    refresh();
}

void AmbulancePage::refresh()
{
    const auto ambulances = AmbulanceService::loadAll();
    const QStringList headers = {"Ambulance ID", "Station / Location", "Status", "Assigned Incident"};

    QVector<QStringList> rows;
    int availableCount = 0;

    for (const auto& a : ambulances) {
        if (a.status.compare("Available", Qt::CaseInsensitive) == 0) availableCount++;

        QString incStr = (a.incidentId > 0) ? QString("Incident #%1").arg(a.incidentId) : "None (Standby)";
        rows.append({
            a.id,
            a.location,
            a.status,
            incStr
        });
    }

    m_dispatchSummary->setText(QString("Fleet Summary: <b>%1 of %2</b> ambulances available for immediate dispatch.")
                               .arg(availableCount).arg(ambulances.size()));

    m_table->setData(headers, rows);

    for (int i = 0; i < ambulances.size(); ++i) {
        const QString st = ambulances[i].status.trimmed();
        const bool isAvail = (st.compare("Available", Qt::CaseInsensitive) == 0);
        m_table->setCellWidget(i, 2, new Badge(st, isAvail ? "ok" : "critical"));
    }
}

void AmbulancePage::onReleaseAmbulance()
{
    auto* table = m_table->table();
    const int r = table->currentRow();
    if (r < 0) {
        QMessageBox::information(this, "Select Ambulance", "Please click an ambulance row to release it.");
        return;
    }

    auto* idItem = table->item(r, 0);
    if (!idItem) return;

    const QString ambId = idItem->text();
    if (AmbulanceService::releaseAmbulance(ambId)) {
        QMessageBox::information(this, "Ambulance Released", QString("Ambulance %1 is now marked Available and its linked incident is Resolved.").arg(ambId));
        refresh();
    }
}

