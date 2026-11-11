#include "MessagesPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../services/MessageService.h"
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

MessagesPage::MessagesPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    auto* topRow = new QHBoxLayout;
    auto* titleLbl = new QLabel(m_ctx.role == "admin" ? "<b>All Messages</b>" : "<b>My Messages</b>");
    titleLbl->setStyleSheet("font-size: 15px;");
    topRow->addWidget(titleLbl);
    topRow->addStretch(1);

    auto* sendBtn = new QPushButton("+ New Message", this);
    sendBtn->setCursor(Qt::PointingHandCursor);
    sendBtn->setFixedHeight(36);
    connect(sendBtn, &QPushButton::clicked, this, &MessagesPage::onSend);
    topRow->addWidget(sendBtn);

    layout->addLayout(topRow);
    m_table = new DataTable(this);
    layout->addWidget(m_table, 1);
    refresh();
}

void MessagesPage::refresh()
{
    const auto msgs = (m_ctx.role == "admin")
        ? MessageService::loadAll()
        : MessageService::loadForUser(m_ctx.userId);

    QStringList headers = {"ID", "Concern", "From", "Role", "To", "Role", "Message"};
    QVector<QStringList> rows;
    for (const auto& m : msgs) {
        rows.append({
            QString::number(m.messageId),
            m.concernId > 0 ? QString::number(m.concernId) : "-",
            m.senderId, m.senderRole,
            m.recipientId, m.recipientRole,
            m.messageText
        });
    }
    m_table->setData(headers, rows);

    for (int i = 0; i < msgs.size(); ++i) {
        QString lvl = "neutral";
        if (msgs[i].senderRole == "admin") lvl = "high";
        else if (msgs[i].senderRole == "faculty") lvl = "info";
        else lvl = "ok";
        m_table->setCellWidget(i, 3, new Badge(msgs[i].senderRole, lvl));
    }
}

void MessagesPage::onSend()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Send Message");
    dlg.setFixedWidth(420);
    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    form->setSpacing(12);

    auto* toEdit = new QLineEdit(&dlg);
    toEdit->setPlaceholderText("Recipient user ID");
    auto* roleBox = new QComboBox(&dlg);
    roleBox->addItems({"student", "faculty", "admin"});
    auto* msgEdit = new QTextEdit(&dlg);
    msgEdit->setFixedHeight(80);
    msgEdit->setPlaceholderText("Type your message...");

    form->addRow("To (User ID):", toEdit);
    form->addRow("Recipient Role:", roleBox);
    form->addRow("Message:", msgEdit);

    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* sendBtn = new QPushButton("Send", &dlg);
    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(sendBtn);
    form->addRow(btnRow);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(sendBtn, &QPushButton::clicked, [&]() {
        if (toEdit->text().trimmed().isEmpty() || msgEdit->toPlainText().trimmed().isEmpty()) {
            QMessageBox::warning(&dlg, "Validation", "Recipient and message are required.");
            return;
        }
        MessageItem m;
        m.concernId = 0;
        m.senderId = m_ctx.userId;
        m.senderRole = m_ctx.role;
        m.recipientId = toEdit->text().trimmed();
        m.recipientRole = roleBox->currentText();
        m.messageText = msgEdit->toPlainText().trimmed().replace('\n', ' ');
        MessageService::sendMessage(m);
        dlg.accept();
    });

    if (dlg.exec() == QDialog::Accepted) refresh();
}

