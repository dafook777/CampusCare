#include "ConcernsPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/ConcernService.h"
#include <QCheckBox>
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

ConcernsPage::ConcernsPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    auto* topRow = new QHBoxLayout;

    auto* filterLbl = new QLabel("Status:");
    filterLbl->setStyleSheet("font-weight: 600; font-size: 13px;");
    topRow->addWidget(filterLbl);

    m_filterBox = new QComboBox(this);
    m_filterBox->addItems({"All", "Open", "In Progress", "Resolved"});
    connect(m_filterBox, &QComboBox::currentIndexChanged, this, &ConcernsPage::onFilterChanged);
    topRow->addWidget(m_filterBox);
    topRow->addStretch(1);

    if (m_ctx.role == "student") {
        auto* submitBtn = new QPushButton("+ Submit Concern", this);
        submitBtn->setCursor(Qt::PointingHandCursor);
        submitBtn->setFixedHeight(36);
        connect(submitBtn, &QPushButton::clicked, this, &ConcernsPage::onSubmit);
        topRow->addWidget(submitBtn);
    }

    if (m_ctx.role == "faculty" || m_ctx.role == "admin") {
        auto* resolveBtn = new QPushButton("Mark Resolved", this);
        resolveBtn->setCursor(Qt::PointingHandCursor);
        resolveBtn->setFixedHeight(36);
        resolveBtn->setStyleSheet("background: #2e7d32; color: white;");
        connect(resolveBtn, &QPushButton::clicked, this, &ConcernsPage::onResolve);
        topRow->addWidget(resolveBtn);
    }

    layout->addLayout(topRow);
    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);
    refresh();
}

void ConcernsPage::refresh()
{
    const auto concerns = ConcernService::loadAll();
    const QString filter = m_filterBox ? m_filterBox->currentText() : "All";

    QStringList headers = {"ID", "Student", "Faculty", "Category", "Description", "Anonymous", "Status"};
    QVector<QStringList> rows;
    QVector<ConcernItem> displayed;

    for (const auto& c : concerns) {
        // Role filter: students see only their own
        if (m_ctx.role == "student" && c.studentId != m_ctx.userId) continue;
        // Faculty sees only assigned to them
        if (m_ctx.role == "faculty" && c.facultyId != m_ctx.userId) continue;
        // Status filter
        if (filter != "All" && !c.status.contains(filter, Qt::CaseInsensitive)) continue;

        displayed.append(c);
        rows.append({
            QString::number(c.id),
            c.isAnonymous ? "Anonymous" : c.studentName,
            c.facultyId,
            c.category,
            c.description,
            c.isAnonymous ? "Yes" : "No",
            c.status
        });
    }

    m_table->setData(headers, rows);
    for (int i = 0; i < displayed.size(); ++i) {
        const auto& c = displayed[i];
        QString lvl = "warn";
        if (c.status.contains("Resolved", Qt::CaseInsensitive)) lvl = "ok";
        else if (c.status.contains("Progress", Qt::CaseInsensitive)) lvl = "info";
        m_table->setCellWidget(i, 6, new Badge(c.status.isEmpty() ? "Open" : c.status, lvl));
    }
}

void ConcernsPage::onFilterChanged() { refresh(); }

void ConcernsPage::onSubmit()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Submit Confidential Concern");
    dlg.setFixedWidth(420);
    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    form->setSpacing(12);

    auto* facultyEdit = new QLineEdit(&dlg);
    facultyEdit->setPlaceholderText("Faculty ID to address concern to");
    auto* catBox = new QComboBox(&dlg);
    catBox->addItems({"academic", "harassment", "mental_health", "facility", "discrimination", "other"});
    auto* descEdit = new QTextEdit(&dlg);
    descEdit->setFixedHeight(80);
    descEdit->setPlaceholderText("Describe your concern...");
    auto* anonCheck = new QCheckBox("Submit anonymously", &dlg);

    form->addRow("Addressed To (Faculty ID):", facultyEdit);
    form->addRow("Category:", catBox);
    form->addRow("Description:", descEdit);
    form->addRow(anonCheck);

    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* saveBtn = new QPushButton("Submit", &dlg);
    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(saveBtn);
    form->addRow(btnRow);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (descEdit->toPlainText().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation", "Description is required.");
            return;
        }
        ConcernItem c;
        c.studentId = m_ctx.userId;
        c.studentName = m_ctx.userId;
        c.facultyId = facultyEdit->text().trimmed();
        c.category = catBox->currentText();
        c.description = descEdit->toPlainText().trimmed().replace('\n', ' ');
        c.isAnonymous = anonCheck->isChecked();
        c.status = "Open";
        ConcernService::addConcern(c);
        dlg.accept();
    });

    if (dlg.exec() == QDialog::Accepted) refresh();
}

void ConcernsPage::onResolve()
{
    auto* table = m_table->table();
    int r = table->currentRow();
    if (r < 0) { QMessageBox::information(this, "Select", "Select a concern row first."); return; }
    auto* item = table->item(r, 0);
    if (!item) return;
    ConcernService::updateStatus(item->text().toInt(), "Resolved");
    refresh();
}

