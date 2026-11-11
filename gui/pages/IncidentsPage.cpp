#include "IncidentsPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/IncidentService.h"
#include "../services/AmbulanceService.h"

#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>

IncidentsPage::IncidentsPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    auto* topRow = new QHBoxLayout;

    auto* filterLabel = new QLabel("Filter Status:");
    filterLabel->setStyleSheet("font-weight: 600; font-size: 13px;");
    topRow->addWidget(filterLabel);

    m_filterBox = new QComboBox(this);
    m_filterBox->addItems({"All Incidents", "Open / Pending", "Assigned", "Resolved"});
    connect(m_filterBox, &QComboBox::currentIndexChanged, this, &IncidentsPage::onFilterChanged);
    topRow->addWidget(m_filterBox);

    topRow->addStretch(1);

    m_addBtn = new QPushButton(QString::fromUtf8("+ Report Incident"), this);
    m_addBtn->setCursor(Qt::PointingHandCursor);
    m_addBtn->setFixedHeight(36);
    connect(m_addBtn, &QPushButton::clicked, this, &IncidentsPage::onAddIncident);
    topRow->addWidget(m_addBtn);

    const bool canManage = (m_ctx.role == "admin" || m_ctx.role == "faculty");

    if (canManage) {
        m_assignBtn = new QPushButton(QString::fromUtf8("\xF0\x9F\x9A\x91 Dispatch Ambulance"), this);
        m_assignBtn->setCursor(Qt::PointingHandCursor);
        m_assignBtn->setFixedHeight(36);
        m_assignBtn->setStyleSheet("background: #0288d1; color: white; font-weight: 600;");
        connect(m_assignBtn, &QPushButton::clicked, this, &IncidentsPage::onAssignAmbulance);
        topRow->addWidget(m_assignBtn);

        m_resolveBtn = new QPushButton(QString::fromUtf8("\xE2\x9C\x93 Resolve Incident"), this);
        m_resolveBtn->setCursor(Qt::PointingHandCursor);
        m_resolveBtn->setFixedHeight(36);
        m_resolveBtn->setStyleSheet("background: #2e7d32; color: white; font-weight: 600;");
        connect(m_resolveBtn, &QPushButton::clicked, this, &IncidentsPage::onResolveIncident);
        topRow->addWidget(m_resolveBtn);
    }

    layout->addLayout(topRow);

    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);

    refresh();
}

void IncidentsPage::refresh()
{
    const auto incidents = IncidentService::loadPriorityQueue();
    const QString filter = m_filterBox ? m_filterBox->currentText() : "All Incidents";

    const QStringList headers = {"ID", "Reporter", "Location", "Type", "Priority", "Status", "Owner", "Description"};

    QVector<QStringList> rows;
    QVector<IncidentItem> displayed;

    for (const auto& inc : incidents) {
        if (m_ctx.role == "student" && inc.reporter != m_ctx.userId && inc.ownerId != m_ctx.userId) continue;

        if (filter == "Open / Pending" && (inc.status.contains("Resolved", Qt::CaseInsensitive) || inc.status.contains("Assigned", Qt::CaseInsensitive))) continue;
        if (filter == "Assigned" && !inc.status.contains("Assigned", Qt::CaseInsensitive)) continue;
        if (filter == "Resolved" && !inc.status.contains("Resolved", Qt::CaseInsensitive)) continue;

        QString prioText;
        switch (inc.severity) {
            case 4: prioText = "Critical (P1)"; break;
            case 3: prioText = "High (P2)"; break;
            case 2: prioText = "Medium (P3)"; break;
            default: prioText = "Low (P4)"; break;
        }

        displayed.append(inc);
        rows.append({
            QString::number(inc.id),
            inc.reporter,
            inc.location,
            inc.type,
            prioText,
            inc.status,
            inc.ownerId,
            inc.description
        });
    }

    m_table->setData(headers, rows);

    for (int i = 0; i < displayed.size(); ++i) {
        const auto& inc = displayed[i];

        QString prioLvl = "neutral";
        QString prioTxt = "Low";
        switch (inc.severity) {
            case 4: prioLvl = "critical"; prioTxt = "Critical"; break;
            case 3: prioLvl = "high";     prioTxt = "High";     break;
            case 2: prioLvl = "medium";   prioTxt = "Medium";   break;
            default: prioLvl = "low";     prioTxt = "Low";      break;
        }
        m_table->setCellWidget(i, 4, new Badge(prioTxt, prioLvl));

        const QString status = inc.status.trimmed();
        QString statLvl = "warn";
        if (status.contains("Resolved", Qt::CaseInsensitive)) statLvl = "ok";
        else if (status.contains("Assigned", Qt::CaseInsensitive)) statLvl = "info";

        m_table->setCellWidget(i, 5, new Badge(status.isEmpty() ? "Open" : status, statLvl));
    }
}

void IncidentsPage::onFilterChanged()
{
    refresh();
}

void IncidentsPage::onAddIncident()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Report New Incident");
    dlg.setFixedWidth(420);

    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    form->setSpacing(12);

    auto* repEdit = new QLineEdit(&dlg);
    repEdit->setText(m_ctx.userId);

    auto* locEdit = new QLineEdit(&dlg);
    locEdit->setPlaceholderText("e.g. CSE dept, Library, Main Gate");

    auto* typeBox = new QComboBox(&dlg);
    typeBox->addItems({"Medical Emergency", "Fire / Hazard", "Harassment / Security", "Facility Issue", "Other"});

    auto* sevBox = new QComboBox(&dlg);
    sevBox->addItem("4 - Critical (Immediate Dispatch)", 4);
    sevBox->addItem("3 - High", 3);
    sevBox->addItem("2 - Medium", 2);
    sevBox->addItem("1 - Low", 1);

    auto* descEdit = new QTextEdit(&dlg);
    descEdit->setFixedHeight(80);
    descEdit->setPlaceholderText("Brief description of the incident...");

    form->addRow("Reporter Name / ID:", repEdit);
    form->addRow("Location:", locEdit);
    form->addRow("Type:", typeBox);
    form->addRow("Priority / Severity:", sevBox);
    form->addRow("Description:", descEdit);

    auto* btnBox = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* submitBtn = new QPushButton("Submit", &dlg);
    btnBox->addStretch(1);
    btnBox->addWidget(cancelBtn);
    btnBox->addWidget(submitBtn);
    form->addRow(btnBox);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(submitBtn, &QPushButton::clicked, [&]() {
        if (locEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation Error", "Please provide a location.");
            return;
        }
        IncidentItem item;
        item.reporter = repEdit->text().trimmed();
        item.location = locEdit->text().trimmed();
        item.type = typeBox->currentText();
        item.severity = sevBox->currentData().toInt();
        item.description = descEdit->toPlainText().trimmed().replace('\n', ' ');
        item.ownerId = m_ctx.userId;
        item.status = "Open";

        if (IncidentService::addIncident(item)) {
            dlg.accept();
        } else {
            QMessageBox::critical(&dlg, "Save Error", "Could not write to incidents.txt");
        }
    });

    if (dlg.exec() == QDialog::Accepted) {
        refresh();
    }
}

void IncidentsPage::onAssignAmbulance()
{
    if (m_ctx.role == "student") return;
    auto* table = m_table->table();
    const int r = table->currentRow();
    if (r < 0) {
        QMessageBox::information(this, "Select Incident", "Please click on an incident row first to dispatch an ambulance.");
        return;
    }

    auto* idItem = table->item(r, 0);
    auto* locItem = table->item(r, 2);
    if (!idItem || !locItem) return;

    const int incId = idItem->text().toInt();
    const QString loc = locItem->text();

    auto res = AmbulanceService::dispatchNearest(incId, loc);
    if (!res.success) {
        QMessageBox::warning(this, "Dispatch Notice", res.errorMsg);
        return;
    }

    QString msg = QString("<b>Ambulance %1 Dispatched!</b><br><br>"
                          "<b>From:</b> %2<br>"
                          "<b>To:</b> %3<br>"
                          "<b>Route:</b> %4<br>"
                          "<b>Distance:</b> %5 m<br>"
                          "<b>ETA:</b> %6 min")
                  .arg(res.ambulanceId)
                  .arg(res.fromLocation)
                  .arg(res.toLocation)
                  .arg(res.route.join(" \xE2\x86\x92 "))
                  .arg(res.metres, 0, 'f', 1)
                  .arg(res.etaMinutes, 0, 'f', 1);

    QMessageBox::information(this, "Ambulance Dispatched", msg);
    refresh();
}

void IncidentsPage::onResolveIncident()
{
    if (m_ctx.role == "student") return;
    auto* table = m_table->table();
    const int r = table->currentRow();
    if (r < 0) {
        QMessageBox::information(this, "Select Incident", "Please click on an incident row first to mark it resolved.");
        return;
    }

    auto* idItem = table->item(r, 0);
    if (!idItem) return;

    const int incId = idItem->text().toInt();
    IncidentService::updateStatus(incId, "Resolved");
    refresh();
}
