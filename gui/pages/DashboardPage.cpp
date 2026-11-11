#include "DashboardPage.h"
#include "../services/IncidentService.h"
#include "../services/AmbulanceService.h"
#include "../services/UserService.h"
#include "../services/StudentService.h"
#include "../services/NotificationService.h"
#include "../services/ConcernService.h"
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

static QFrame* makeKpiCard(const QString& title, QLabel*& valueLbl, const QString& color)
{
    auto* card = new QFrame;
    card->setObjectName("card");
    card->setMinimumSize(200, 110);
    auto* vbox = new QVBoxLayout(card);
    vbox->setContentsMargins(18, 14, 18, 14);
    vbox->setSpacing(6);

    auto* titleLbl = new QLabel(title);
    titleLbl->setStyleSheet("font-size: 12px; font-weight: 600; color: #8b97a7; background: transparent;");
    vbox->addWidget(titleLbl);

    valueLbl = new QLabel("0");
    valueLbl->setStyleSheet(QString("font-size: 32px; font-weight: 700; color: %1; background: transparent;").arg(color));
    vbox->addWidget(valueLbl);
    vbox->addStretch(1);
    return card;
}

DashboardPage::DashboardPage(const AppContext& ctx, QWidget* parent)
    : BasePage(parent), m_ctx(ctx)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(20);

    auto* header = new QLabel(QString::fromUtf8("<b>\xF0\x9F\x9B\xA1 CampusCare Admin Dashboard</b>"));
    header->setStyleSheet("font-size: 18px;");
    layout->addWidget(header);

    auto* grid = new QGridLayout;
    grid->setSpacing(16);

    grid->addWidget(makeKpiCard("TOTAL INCIDENTS", m_incTotal, "#03a9f4"), 0, 0);
    grid->addWidget(makeKpiCard("OPEN / PENDING", m_incOpen, "#ef5350"), 0, 1);
    grid->addWidget(makeKpiCard("RESOLVED", m_incResolved, "#66bb6a"), 0, 2);
    grid->addWidget(makeKpiCard("AMBULANCES AVAILABLE", m_ambAvail, "#66bb6a"), 1, 0);
    grid->addWidget(makeKpiCard("AMBULANCES BUSY", m_ambBusy, "#ffa726"), 1, 1);
    grid->addWidget(makeKpiCard("REGISTERED USERS", m_userCount, "#03a9f4"), 1, 2);
    grid->addWidget(makeKpiCard("STUDENT PROFILES", m_studentCount, "#7e57c2"), 2, 0);
    grid->addWidget(makeKpiCard("UNREAD NOTIFICATIONS", m_notifUnread, "#ef5350"), 2, 1);
    grid->addWidget(makeKpiCard("OPEN CONCERNS", m_concernCount, "#ffa726"), 2, 2);

    layout->addLayout(grid);
    layout->addStretch(1);
    refresh();
}

void DashboardPage::refresh()
{
    const auto incidents = IncidentService::loadAll();
    int open = 0, resolved = 0;
    for (const auto& i : incidents) {
        if (i.status.contains("Resolved", Qt::CaseInsensitive)) ++resolved;
        else ++open;
    }
    m_incTotal->setText(QString::number(incidents.size()));
    m_incOpen->setText(QString::number(open));
    m_incResolved->setText(QString::number(resolved));

    const auto ambs = AmbulanceService::loadAll();
    int avail = 0, busy = 0;
    for (const auto& a : ambs) {
        if (a.status.contains("Available", Qt::CaseInsensitive)) ++avail;
        else ++busy;
    }
    m_ambAvail->setText(QString::number(avail));
    m_ambBusy->setText(QString::number(busy));

    m_userCount->setText(QString::number(UserService::loadAll().size()));
    m_studentCount->setText(QString::number(StudentService::loadAll().size()));
    m_notifUnread->setText(QString::number(NotificationService::countUnread(m_ctx.userId)));

    const auto concerns = ConcernService::loadAll();
    int openConcerns = 0;
    for (const auto& c : concerns)
        if (!c.status.contains("Resolved", Qt::CaseInsensitive)) ++openConcerns;
    m_concernCount->setText(QString::number(openConcerns));
}

