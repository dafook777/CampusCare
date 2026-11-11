#include "ContactsPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/ContactService.h"
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>

ContactsPage::ContactsPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    auto* topRow = new QHBoxLayout;
    auto* titleLbl = new QLabel("<b>Campus Emergency Contacts Directory</b>");
    titleLbl->setStyleSheet("font-size: 15px;");
    topRow->addWidget(titleLbl);
    topRow->addStretch(1);

    if (m_ctx.role == "admin") {
        auto* addBtn = new QPushButton("+ Add Contact", this);
        addBtn->setCursor(Qt::PointingHandCursor);
        addBtn->setFixedHeight(36);
        connect(addBtn, &QPushButton::clicked, this, &ContactsPage::onAdd);
        topRow->addWidget(addBtn);

        auto* delBtn = new QPushButton("Delete Selected", this);
        delBtn->setCursor(Qt::PointingHandCursor);
        delBtn->setFixedHeight(36);
        delBtn->setStyleSheet("background: #d32f2f; color: white;");
        connect(delBtn, &QPushButton::clicked, this, &ContactsPage::onDelete);
        topRow->addWidget(delBtn);
    }
    layout->addLayout(topRow);

    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);
    refresh();
}

void ContactsPage::refresh()
{
    const auto contacts = ContactService::loadAll();
    const QStringList headers = {"Name", "Phone Number", "Category"};
    QVector<QStringList> rows;
    for (const auto& c : contacts)
        rows.append({c.name, c.number, c.category});
    m_table->setData(headers, rows);

    for (int i = 0; i < contacts.size(); ++i) {
        const auto& c = contacts[i];
        QString lvl = "info";
        if (c.category.contains("Police", Qt::CaseInsensitive) || c.category.contains("Security", Qt::CaseInsensitive))
            lvl = "high";
        else if (c.category.contains("Medical", Qt::CaseInsensitive) || c.category.contains("Hospital", Qt::CaseInsensitive))
            lvl = "critical";
        else if (c.category.contains("Fire", Qt::CaseInsensitive))
            lvl = "warn";
        m_table->setCellWidget(i, 2, new Badge(c.category, lvl));
    }
}

void ContactsPage::onAdd()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Add Emergency Contact");
    dlg.setFixedWidth(380);
    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    form->setSpacing(12);

    auto* nameEdit = new QLineEdit(&dlg);
    auto* phoneEdit = new QLineEdit(&dlg);
    auto* catBox = new QComboBox(&dlg);
    catBox->addItems({"Campus Security", "Medical Center", "Fire Department", "Police", "Warden", "Counselor", "Other"});

    form->addRow("Name:", nameEdit);
    form->addRow("Phone:", phoneEdit);
    form->addRow("Category:", catBox);

    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* saveBtn = new QPushButton("Save", &dlg);
    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(saveBtn);
    form->addRow(btnRow);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (nameEdit->text().trimmed().isEmpty() || phoneEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation", "Name and phone are required.");
            return;
        }
        ContactItem c;
        c.name = nameEdit->text().trimmed();
        c.number = phoneEdit->text().trimmed();
        c.category = catBox->currentText();
        ContactService::addContact(c);
        dlg.accept();
    });

    if (dlg.exec() == QDialog::Accepted) refresh();
}

void ContactsPage::onDelete()
{
    auto* table = m_table->table();
    int r = table->currentRow();
    if (r < 0) { QMessageBox::information(this, "Select", "Select a contact row first."); return; }
    auto* item = table->item(r, 0);
    if (!item) return;
    ContactService::deleteContact(item->text());
    refresh();
}

