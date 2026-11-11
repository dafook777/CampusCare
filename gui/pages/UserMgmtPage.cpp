#include "UserMgmtPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/UserService.h"
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

UserMgmtPage::UserMgmtPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    auto* topRow = new QHBoxLayout;
    auto* titleLbl = new QLabel("<b>User Accounts</b>");
    titleLbl->setStyleSheet("font-size: 15px;");
    topRow->addWidget(titleLbl);
    topRow->addStretch(1);

    auto* addBtn = new QPushButton("+ Add User", this);
    addBtn->setCursor(Qt::PointingHandCursor);
    addBtn->setFixedHeight(36);
    connect(addBtn, &QPushButton::clicked, this, &UserMgmtPage::onAddUser);
    topRow->addWidget(addBtn);

    auto* roleBtn = new QPushButton("Change Role", this);
    roleBtn->setCursor(Qt::PointingHandCursor);
    roleBtn->setFixedHeight(36);
    roleBtn->setStyleSheet("background: #f57c00; color: white;");
    connect(roleBtn, &QPushButton::clicked, this, &UserMgmtPage::onChangeRole);
    topRow->addWidget(roleBtn);

    auto* delBtn = new QPushButton("Delete User", this);
    delBtn->setCursor(Qt::PointingHandCursor);
    delBtn->setFixedHeight(36);
    delBtn->setStyleSheet("background: #d32f2f; color: white;");
    connect(delBtn, &QPushButton::clicked, this, &UserMgmtPage::onDeleteUser);
    topRow->addWidget(delBtn);

    layout->addLayout(topRow);
    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);
    refresh();
}

void UserMgmtPage::refresh()
{
    const auto users = UserService::loadAll();
    QStringList headers = {"User ID", "Role", "Has Password"};
    QVector<QStringList> rows;
    for (const auto& u : users)
        rows.append({u.userId, u.role, u.password.isEmpty() ? "No" : "Yes"});
    m_table->setData(headers, rows);

    for (int i = 0; i < users.size(); ++i) {
        QString lvl = "neutral";
        if (users[i].role == "admin") lvl = "high";
        else if (users[i].role == "faculty") lvl = "info";
        else lvl = "ok";
        m_table->setCellWidget(i, 1, new Badge(users[i].role, lvl));
    }
}

void UserMgmtPage::onAddUser()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Add New User");
    dlg.setFixedWidth(380);
    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    form->setSpacing(12);

    auto* idEdit = new QLineEdit(&dlg);
    idEdit->setPlaceholderText("e.g. 25101211030");
    auto* roleBox = new QComboBox(&dlg);
    roleBox->addItems({"student", "faculty", "admin"});
    auto* passEdit = new QLineEdit(&dlg);
    passEdit->setPlaceholderText("Leave empty for no password");

    form->addRow("User ID:", idEdit);
    form->addRow("Role:", roleBox);
    form->addRow("Password:", passEdit);

    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* saveBtn = new QPushButton("Create", &dlg);
    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(saveBtn);
    form->addRow(btnRow);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (idEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation", "User ID is required.");
            return;
        }
        UserItem u;
        u.userId = idEdit->text().trimmed();
        u.role = roleBox->currentText();
        u.password = passEdit->text().trimmed();
        if (!UserService::addUser(u)) {
            QMessageBox::warning(&dlg, "Error", "User already exists.");
            return;
        }
        dlg.accept();
    });

    if (dlg.exec() == QDialog::Accepted) refresh();
}

void UserMgmtPage::onDeleteUser()
{
    auto* table = m_table->table();
    int r = table->currentRow();
    if (r < 0) { QMessageBox::information(this, "Select", "Select a user row first."); return; }
    auto* item = table->item(r, 0);
    if (!item) return;
    const QString uid = item->text();
    if (uid == m_ctx.userId) { QMessageBox::warning(this, "Error", "Cannot delete yourself."); return; }
    if (QMessageBox::question(this, "Confirm", "Delete user " + uid + "?") == QMessageBox::Yes) {
        UserService::deleteUser(uid);
        refresh();
    }
}

void UserMgmtPage::onChangeRole()
{
    auto* table = m_table->table();
    int r = table->currentRow();
    if (r < 0) { QMessageBox::information(this, "Select", "Select a user row first."); return; }
    auto* item = table->item(r, 0);
    if (!item) return;
    const QString uid = item->text();

    QDialog dlg(this);
    dlg.setWindowTitle("Change Role for " + uid);
    dlg.setFixedWidth(300);
    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    auto* roleBox = new QComboBox(&dlg);
    roleBox->addItems({"student", "faculty", "admin"});
    form->addRow("New Role:", roleBox);

    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* saveBtn = new QPushButton("Update", &dlg);
    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(saveBtn);
    form->addRow(btnRow);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        UserService::changeRole(uid, roleBox->currentText());
        dlg.accept();
    });

    if (dlg.exec() == QDialog::Accepted) refresh();
}

