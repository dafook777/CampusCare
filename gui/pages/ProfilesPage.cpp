#include "ProfilesPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/StudentService.h"
#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

ProfilesPage::ProfilesPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    if (m_ctx.role == "student") {
        // Show personal profile card
        m_profileCard = new QFrame(this);
        m_profileCard->setObjectName("card");
        auto* cardLayout = new QFormLayout(m_profileCard);
        cardLayout->setContentsMargins(20, 16, 20, 16);
        cardLayout->setSpacing(10);
        layout->addWidget(m_profileCard);

        auto* editBtn = new QPushButton("Edit Profile", this);
        editBtn->setCursor(Qt::PointingHandCursor);
        editBtn->setFixedHeight(36);
        editBtn->setFixedWidth(160);
        connect(editBtn, &QPushButton::clicked, this, &ProfilesPage::onEdit);
        layout->addWidget(editBtn);
        layout->addStretch(1);
    } else {
        // Faculty/Admin: show all students table
        m_table = new DataTable(this);
        layout->addWidget(m_table, 1);
    }
    refresh();
}

void ProfilesPage::refresh()
{
    if (m_ctx.role == "student") {
        auto s = StudentService::getById(m_ctx.userId);
        // Clear and rebuild the profile card
        auto* cardLayout = qobject_cast<QFormLayout*>(m_profileCard->layout());
        if (!cardLayout) return;
        // Remove existing rows
        while (cardLayout->rowCount() > 0)
            cardLayout->removeRow(0);

        auto makeVal = [](const QString& text) {
            auto* l = new QLabel(text);
            l->setStyleSheet("font-size: 14px; font-weight: 600;");
            return l;
        };
        auto makeLbl = [](const QString& text) {
            auto* l = new QLabel(text);
            l->setStyleSheet("font-size: 13px; color: #8b97a7;");
            return l;
        };

        cardLayout->addRow(makeLbl("Student ID:"), makeVal(s.studentId.isEmpty() ? m_ctx.userId : s.studentId));
        cardLayout->addRow(makeLbl("Name:"), makeVal(s.name.isEmpty() ? "(not set)" : s.name));
        cardLayout->addRow(makeLbl("Department:"), makeVal(s.department.isEmpty() ? "(not set)" : s.department));
        cardLayout->addRow(makeLbl("Phone:"), makeVal(s.phone.isEmpty() ? "(not set)" : s.phone));
        cardLayout->addRow(makeLbl("Blood Group:"), makeVal(s.bloodGroup.isEmpty() ? "(not set)" : s.bloodGroup));
        cardLayout->addRow(makeLbl("Gender:"), makeVal(s.gender.isEmpty() ? "(not set)" : s.gender));
        cardLayout->addRow(makeLbl("Status:"), new Badge(s.isActive ? "Active" : "Inactive", s.isActive ? "ok" : "warn"));
    } else {
        const auto students = StudentService::loadAll();
        QStringList headers = {"ID", "Name", "Department", "Phone", "Blood Group", "Gender", "Status"};
        QVector<QStringList> rows;
        for (const auto& s : students) {
            rows.append({s.studentId, s.name, s.department, s.phone, s.bloodGroup, s.gender,
                         s.isActive ? "Active" : "Inactive"});
        }
        m_table->setData(headers, rows);
        for (int i = 0; i < students.size(); ++i) {
            m_table->setCellWidget(i, 6, new Badge(
                students[i].isActive ? "Active" : "Inactive",
                students[i].isActive ? "ok" : "warn"));
        }
    }
}

void ProfilesPage::onEdit()
{
    auto s = StudentService::getById(m_ctx.userId);
    QDialog dlg(this);
    dlg.setWindowTitle("Edit My Profile");
    dlg.setFixedWidth(400);
    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    form->setSpacing(12);

    auto* nameEdit = new QLineEdit(s.name, &dlg);
    auto* deptEdit = new QLineEdit(s.department, &dlg);
    auto* phoneEdit = new QLineEdit(s.phone, &dlg);
    auto* bloodEdit = new QComboBox(&dlg);
    bloodEdit->addItems({"Unknown", "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"});
    bloodEdit->setCurrentText(s.bloodGroup.isEmpty() ? "Unknown" : s.bloodGroup);
    auto* genderBox = new QComboBox(&dlg);
    genderBox->addItems({"Male", "Female", "Other"});
    genderBox->setCurrentText(s.gender.isEmpty() ? "Male" : s.gender);

    form->addRow("Name:", nameEdit);
    form->addRow("Department:", deptEdit);
    form->addRow("Phone:", phoneEdit);
    form->addRow("Blood Group:", bloodEdit);
    form->addRow("Gender:", genderBox);

    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* saveBtn = new QPushButton("Save", &dlg);
    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(saveBtn);
    form->addRow(btnRow);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        StudentItem updated;
        updated.studentId = m_ctx.userId;
        updated.name = nameEdit->text().trimmed();
        updated.department = deptEdit->text().trimmed();
        updated.phone = phoneEdit->text().trimmed();
        updated.bloodGroup = bloodEdit->currentText();
        updated.gender = genderBox->currentText();
        updated.isActive = true;
        StudentService::update(updated);
        dlg.accept();
    });

    if (dlg.exec() == QDialog::Accepted) refresh();
}

