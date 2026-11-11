#include "DispatchLogPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/DispatchLogService.h"
#include <QVBoxLayout>
#include <QLabel>

DispatchLogPage::DispatchLogPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    auto* titleLbl = new QLabel("<b>Ambulance Dispatch History</b>");
    titleLbl->setStyleSheet("font-size: 15px;");
    layout->addWidget(titleLbl);

    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);
    refresh();
}

void DispatchLogPage::refresh()
{
    const auto lines = DispatchLogService::loadAll();
    // Parse: "2026-11-03 19:30:36 | SUCCESS | incident=1003 | ambulance=AMB-102 | ..."
    QStringList headers = {"Date/Time", "Status", "Incident", "Ambulance", "Details"};
    QVector<QStringList> rows;
    QStringList statuses;

    for (const auto& line : lines) {
        const QStringList parts = line.split('|');
        if (parts.size() >= 3) {
            QString dateTime = parts[0].trimmed();
            QString status = parts[1].trimmed();
            QString incident;
            QString ambulance;
            QStringList details;

            for (int i = 2; i < parts.size(); ++i) {
                QString part = parts[i].trimmed();
                if (part.startsWith("incident="))
                    incident = part.mid(9);
                else if (part.startsWith("ambulance="))
                    ambulance = part.mid(10);
                else
                    details.append(part);
            }
            rows.append({dateTime, status, incident, ambulance, details.join(", ")});
            statuses.append(status);
        }
    }

    m_table->setData(headers, rows);
    for (int i = 0; i < statuses.size(); ++i) {
        QString lvl = "info";
        if (statuses[i] == "SUCCESS") lvl = "ok";
        else if (statuses[i] == "COMPLETED") lvl = "info";
        else if (statuses[i].contains("FAIL")) lvl = "high";
        m_table->setCellWidget(i, 1, new Badge(statuses[i], lvl));
    }
}

