#include "LoginWindow.h"
#include "services/DataPath.h"
#include "services/IncidentService.h"
#include "services/NotificationService.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSet>
#include <QTextStream>
#include <QVBoxLayout>
#include <QTimer>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QKeyEvent>
#include <cmath>
#include <algorithm>

namespace {

const char* kCardStyle =
    "QFrame#card {"
    "  background: rgba(18, 25, 38, 225);"
    "  border: 1px solid rgba(255, 255, 255, 35);"
    "  border-radius: 18px;"
    "}"
    "QFrame#cardHeader {"
    "  background: rgba(24, 34, 50, 235);"
    "  border-top-left-radius: 17px;"
    "  border-top-right-radius: 17px;"
    "  border-bottom: 1px solid rgba(255, 255, 255, 25);"
    "}"
    "QLabel#logoText {"
    "  color: #f8fafc; font-size: 16px; font-weight: 800; letter-spacing: 0.5px; background: transparent;"
    "}"
    "QLabel#subLogo {"
    "  color: #00e5ff; font-size: 11px; font-weight: 600; letter-spacing: 1px; background: transparent;"
    "}"
    "QLabel#fieldIcon {"
    "  background: rgba(30, 41, 59, 230);"
    "  border: 1px solid rgba(255, 255, 255, 25);"
    "  border-right: none;"
    "  border-top-left-radius: 10px;"
    "  border-bottom-left-radius: 10px;"
    "  color: #94a3b8; font-size: 18px;"
    "}"
    "QFrame#field {"
    "  background: rgba(15, 23, 42, 230);"
    "  border: 1px solid rgba(255, 255, 255, 25);"
    "  border-top-right-radius: 10px;"
    "  border-bottom-right-radius: 10px;"
    "}"
    "QFrame#field:focus-within {"
    "  border: 1px solid #00e5ff;"
    "}"
    "QLineEdit {"
    "  background: transparent; border: none; color: #f8fafc; font-size: 15px; font-weight: 500;"
    "}"
    "QLineEdit:focus { border: none; }"
    "QPushButton#eyeBtn {"
    "  border: none; background: transparent; font-size: 16px; padding: 0 8px; color: #94a3b8;"
    "}"
    "QPushButton#eyeBtn:checked { color: #00e5ff; }"
    "QPushButton#loginBtn {"
    "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0288d1, stop:1 #00e5ff);"
    "  color: #0a0f1d; border: none; border-radius: 10px; font-size: 15px; font-weight: 700;"
    "  letter-spacing: 1px;"
    "}"
    "QPushButton#loginBtn:hover {"
    "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #03a9f4, stop:1 #38bdf8);"
    "}"
    "QPushButton#loginBtn:pressed {"
    "  background: #0288d1;"
    "}"
    "QLabel#roleBadge {"
    "  border-radius: 6px; padding: 5px 12px; font-size: 11px; font-weight: 600;"
    "}"
    "QLabel#errorLabel {"
    "  color: #fca5a5; font-size: 12px; font-weight: 500; background: rgba(239, 68, 68, 30);"
    "  border: 1px solid rgba(239, 68, 68, 80); border-radius: 6px; padding: 6px 10px;"
    "}"
    "QPushButton#quickChip {"
    "  background: rgba(30, 41, 59, 190); color: #94a3b8; border: 1px solid rgba(255, 255, 255, 20);"
    "  border-radius: 6px; font-size: 11px; font-weight: 600; padding: 4px 10px;"
    "}"
    "QPushButton#quickChip:hover {"
    "  background: rgba(56, 189, 248, 40); color: #38bdf8; border: 1px solid rgba(56, 189, 248, 80);"
    "}";

// Helper to draw authentic Graphic Era Hill University Crest
static QPixmap createUniversityCrest(int size = 52)
{
    QPixmap px(size, size);
    px.fill(Qt::transparent);
    QPainter p(&px);
    p.setRenderHint(QPainter::Antialiasing);

    const double s = size / 100.0;
    const double cx = 50.0 * s, cy = 46.0 * s;

    // Laurel Wreath Leaves (Left & Right)
    p.setPen(QPen(QColor("#fde047"), 1.2 * s));
    p.setBrush(QColor("#eab308"));
    for (int side : {-1, 1}) {
        for (int i = 0; i < 7; ++i) {
            double angle = -1.15 + i * 0.36;
            double lx = cx + side * (26.0 + 5.0 * std::cos(angle)) * s;
            double ly = (cy + 22.0 * std::sin(angle) + 4.0) * s;
            p.drawEllipse(QPointF(lx, ly), 3.2 * s, 1.9 * s);
        }
    }

    // Outer Gold Crest
    QPolygonF outerShield;
    outerShield << QPointF(cx - 20 * s, cy - 24 * s)
                << QPointF(cx + 20 * s, cy - 24 * s)
                << QPointF(cx + 20 * s, cy + 4 * s)
                << QPointF(cx, cy + 26 * s)
                << QPointF(cx - 20 * s, cy + 4 * s);
    p.setBrush(QColor("#eab308"));
    p.setPen(QPen(QColor("#fef08a"), 1.0 * s));
    p.drawPolygon(outerShield);

    // Inner Crimson Shield
    QPolygonF innerShield;
    innerShield << QPointF(cx - 17 * s, cy - 21 * s)
                << QPointF(cx + 17 * s, cy - 21 * s)
                << QPointF(cx + 17 * s, cy + 3 * s)
                << QPointF(cx, cy + 23 * s)
                << QPointF(cx - 17 * s, cy + 3 * s);
    p.setBrush(QColor("#991b1b"));
    p.setPen(QPen(QColor("#ca8a04"), 1.0 * s));
    p.drawPolygon(innerShield);

    // Upper Section Division: Deep Navy
    QPolygonF upperDiv;
    upperDiv << QPointF(cx - 17 * s, cy - 21 * s)
             << QPointF(cx + 17 * s, cy - 21 * s)
             << QPointF(cx + 17 * s, cy - 6 * s)
             << QPointF(cx - 17 * s, cy - 6 * s);
    p.setBrush(QColor("#0f172a"));
    p.setPen(Qt::NoPen);
    p.drawPolygon(upperDiv);

    // Torch of Knowledge (Flame & Handle)
    p.setBrush(QColor("#ef4444"));
    p.drawEllipse(QPointF(cx, cy - 14 * s), 2.8 * s, 4.5 * s);
    p.setBrush(QColor("#fde047"));
    p.drawEllipse(QPointF(cx, cy - 13 * s), 1.5 * s, 2.5 * s);
    p.setBrush(QColor("#eab308"));
    p.drawRect(QRectF(cx - 1.2 * s, cy - 10 * s, 2.4 * s, 4.5 * s));

    // Open Book of Knowledge
    p.setBrush(QColor("#ffffff"));
    p.setPen(QPen(QColor("#ca8a04"), 0.8 * s));
    QPolygonF bookL, bookR;
    bookL << QPointF(cx - 12 * s, cy + 2 * s) << QPointF(cx - 1 * s, cy + 4 * s)
          << QPointF(cx - 1 * s, cy + 12 * s) << QPointF(cx - 12 * s, cy + 10 * s);
    bookR << QPointF(cx + 12 * s, cy + 2 * s) << QPointF(cx + 1 * s, cy + 4 * s)
          << QPointF(cx + 1 * s, cy + 12 * s) << QPointF(cx + 12 * s, cy + 10 * s);
    p.drawPolygon(bookL);
    p.drawPolygon(bookR);

    // Gold Banner below shield with "GEHU"
    QPolygonF banner;
    banner << QPointF(cx - 24 * s, cy + 25 * s)
           << QPointF(cx + 24 * s, cy + 25 * s)
           << QPointF(cx + 21 * s, cy + 32 * s)
           << QPointF(cx - 21 * s, cy + 32 * s);
    p.setBrush(QColor("#eab308"));
    p.setPen(QPen(QColor("#fef08a"), 0.8 * s));
    p.drawPolygon(banner);

    p.setFont(QFont("Segoe UI", 6 * s, QFont::Bold));
    p.setPen(QColor("#7f1d1d"));
    p.drawText(QRectF(cx - 20 * s, cy + 24 * s, 40 * s, 9 * s), Qt::AlignCenter, "GEHU");

    return px;
}

// Helper to retrieve official Graphic Era Hill University logo
static QPixmap getUniversityLogo(int size = 52)
{
    static QMap<int, QPixmap> s_cache;
    if (s_cache.contains(size)) return s_cache[size];

    QPixmap raw;
    if (QFile::exists(":/gehu_logo.png")) {
        raw.load(":/gehu_logo.png");
    } else if (QFile::exists(":/gehu_logo.jpg")) {
        raw.load(":/gehu_logo.jpg");
    }
    if (raw.isNull()) {
        raw.load(DataPath::resolve("resources/gehu_logo.png"));
    }
    if (raw.isNull()) {
        raw.load(DataPath::resolve("resources/gehu_logo.jpg"));
    }
    if (raw.isNull()) {
        raw.load(DataPath::resolve("gehu_logo.png"));
    }

    if (raw.isNull()) {
        return createUniversityCrest(size);
    }

    QPixmap scaled = raw.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap result(size, size);
    result.fill(Qt::transparent);
    QPainter p(&result);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPainterPath clip;
    clip.addEllipse(0.5, 0.5, size - 1.0, size - 1.0);
    p.setClipPath(clip);
    p.drawPixmap(0, 0, scaled);
    p.end();

    s_cache[size] = result;
    return result;
}

bool lookupUser(const QString& id, QString& role, QString& password)
{
    QFile f(DataPath::resolve("users.txt"));
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&f);
    while (!in.atEnd()) {
        const QStringList t = in.readLine().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (t.size() >= 2 && t[0] == id) {
            role = t[1];
            password = (t.size() >= 3) ? t[2] : "";
            return true;
        }
    }
    return false;
}

} // namespace

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent), m_bg(":/campus_bg.jpg")
{
    setWindowTitle("CampusCare — Student Safety & Emergency Response");
    setMinimumSize(1024, 640);
    resize(1280, 720);
    setMouseTracking(true);
    setStyleSheet(kCardStyle);

    setupUI();

    // Start in Intro Mode
    m_stage = StageIntro;
    m_cardWidget->hide();

    // Background & Animation Timer (~30 FPS, ultra-low CPU)
    m_bgTimer = new QTimer(this);
    connect(m_bgTimer, &QTimer::timeout, this, &LoginWindow::onBgTimerTick);
    m_bgTimer->start(33);

    // Skip intro button
    m_skipBtn = new QPushButton("Enter Portal ➔", this);
    m_skipBtn->setCursor(Qt::PointingHandCursor);
    m_skipBtn->setStyleSheet(
        "background: rgba(15, 23, 42, 200); color: #94a3b8; border: 1px solid rgba(255,255,255,30);"
        "border-radius: 16px; padding: 6px 14px; font-size: 11px; font-weight: 600;"
    );
    m_skipBtn->adjustSize();
    connect(m_skipBtn, &QPushButton::clicked, this, &LoginWindow::skipIntro);

    // Build the critical alerts panel on the login screen
    buildAlertPanel();
}

void LoginWindow::buildAlertPanel()
{
    m_alerts.clear();

    // 1. Critical incidents (severity >= 4)
    const auto allIncidents = IncidentService::loadAll();
    QSet<int> seenIncidentIds;
    for (const auto& inc : allIncidents) {
        if (inc.severity >= 4) {
            LoginAlertItem a;
            a.incidentId = inc.id;
            a.icon = QString::fromUtf8("\xF0\x9F\x9A\xA8"); // 🚨
            a.category = "CRITICAL INCIDENT";
            a.location = inc.location;
            a.type = inc.type.toUpper();
            a.description = inc.description;
            a.reporter = inc.reporter;
            a.status = inc.status;
            a.expanded = false;
            m_alerts.append(a);
            seenIncidentIds.insert(inc.id);
        }
    }

    // 2. Ambulance Dispatches from notifications
    const auto allNotifs = NotificationService::loadAll();
    QSet<int> seenDispatchIncidents;
    for (const auto& n : allNotifs) {
        if (n.notificationType == "Ambulance Dispatched" && !seenDispatchIncidents.contains(n.incidentId)) {
            seenDispatchIncidents.insert(n.incidentId);
            LoginAlertItem a;
            a.incidentId = n.incidentId;
            a.icon = QString::fromUtf8("\xF0\x9F\x9A\x91"); // 🚑
            a.category = "AMBULANCE DISPATCHED";
            auto inc = IncidentService::getById(n.incidentId);
            a.location = inc.location.isEmpty() ? "Campus Ground / Main Gate" : inc.location;
            a.type = "MEDICAL RESPONSE";
            a.description = n.message;
            a.reporter = inc.reporter.isEmpty() ? "Emergency Response Unit" : inc.reporter;
            a.status = "En Route / Dispatched";
            a.expanded = false;
            m_alerts.append(a);
        }
    }

    if (m_alerts.isEmpty()) return;

    // Create the floating alert panel
    m_alertPanel = new QWidget(this);
    m_alertPanel->setFixedWidth(340);
    m_alertPanel->setStyleSheet("background: transparent;");

    auto* panelLayout = new QVBoxLayout(m_alertPanel);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(8);

    // Header badge
    auto* headerBadge = new QLabel(
        QString::fromUtf8("\xF0\x9F\x9A\xA8  LIVE EMERGENCY BROADCAST  (%1)").arg(m_alerts.size()),
        m_alertPanel
    );
    headerBadge->setAlignment(Qt::AlignCenter);
    headerBadge->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(220, 38, 38, 200), stop:1 rgba(185, 28, 28, 200));"
        "color: #ffffff; border: 1px solid rgba(248, 113, 113, 160);"
        "border-radius: 10px; padding: 7px 14px; font-size: 11px; font-weight: 800; letter-spacing: 0.8px;"
    );
    panelLayout->addWidget(headerBadge);

    // Scrollable alert list
    m_alertScroll = new QScrollArea(m_alertPanel);
    m_alertScroll->setWidgetResizable(true);
    m_alertScroll->setFrameShape(QFrame::NoFrame);
    m_alertScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_alertScroll->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollBar:vertical { background: rgba(15, 23, 42, 100); width: 4px; border-radius: 2px; }"
        "QScrollBar::handle:vertical { background: rgba(239, 68, 68, 140); border-radius: 2px; min-height: 20px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
    );

    m_alertContent = new QWidget;
    m_alertContent->setStyleSheet("background: transparent;");
    m_alertLayout = new QVBoxLayout(m_alertContent);
    m_alertLayout->setContentsMargins(0, 0, 0, 0);
    m_alertLayout->setSpacing(8);

    for (int i = 0; i < m_alerts.size(); ++i) {
        const auto& a = m_alerts[i];

        auto* alertCard = new QFrame(m_alertContent);
        alertCard->setObjectName("alertCard");
        alertCard->setCursor(Qt::PointingHandCursor);
        const bool isAmbulance = (a.category == "AMBULANCE DISPATCHED");
        const QString borderColor = isAmbulance ? "rgba(56, 189, 248, 120)" : "rgba(239, 68, 68, 120)";
        const QString hoverBorder = isAmbulance ? "#38bdf8" : "#ef4444";
        const QString tagBg = isAmbulance ? "rgba(2, 132, 199, 45)" : "rgba(220, 38, 38, 45)";
        const QString tagColor = isAmbulance ? "#38bdf8" : "#fca5a5";

        alertCard->setStyleSheet(QString(
            "QFrame#alertCard {"
            "  background: rgba(15, 23, 42, 230);"
            "  border: 1px solid %1;"
            "  border-radius: 12px;"
            "}"
            "QFrame#alertCard:hover {"
            "  border: 1px solid %2;"
            "  background: rgba(24, 34, 53, 245);"
            "}"
        ).arg(borderColor, hoverBorder));

        auto* cardVBox = new QVBoxLayout(alertCard);
        cardVBox->setContentsMargins(12, 10, 12, 10);
        cardVBox->setSpacing(5);

        // Top tag row: Icon + Category Badge + ID + Arrow
        auto* tagHBox = new QHBoxLayout;
        tagHBox->setSpacing(6);

        auto* catBadge = new QLabel(QString("%1  %2").arg(a.icon, a.category), alertCard);
        catBadge->setStyleSheet(QString(
            "background: %1; color: %2; font-size: 10px; font-weight: 800; border-radius: 6px; padding: 2px 8px;"
        ).arg(tagBg, tagColor));
        tagHBox->addWidget(catBadge);

        auto* idBadge = new QLabel(QString("INC-%1").arg(a.incidentId), alertCard);
        idBadge->setStyleSheet(
            "color: #94a3b8; font-family: 'Consolas', monospace; font-size: 10px; font-weight: 700;"
        );
        tagHBox->addWidget(idBadge);
        tagHBox->addStretch(1);

        auto* toggleIndicator = new QLabel(QString::fromUtf8("\xE2\x96\xBC"), alertCard);
        toggleIndicator->setStyleSheet("color: #94a3b8; font-size: 10px; font-weight: 700;");
        tagHBox->addWidget(toggleIndicator);

        cardVBox->addLayout(tagHBox);

        // Summary Headline: Location + Type
        auto* headline = new QLabel(QString("%1 at %2").arg(a.type, a.location), alertCard);
        headline->setStyleSheet("color: #f8fafc; font-size: 13px; font-weight: 700;");
        headline->setWordWrap(true);
        cardVBox->addWidget(headline);

        // Click-to-read hint
        auto* hintText = new QLabel("Click to read full bulletin \xE2\x96\xBE", alertCard);
        hintText->setStyleSheet("color: #64748b; font-size: 10px; font-weight: 600;");
        cardVBox->addWidget(hintText);

        // Expandable Detail Widget
        auto* detailWidget = new QWidget(alertCard);
        detailWidget->hide();
        auto* detailVBox = new QVBoxLayout(detailWidget);
        detailVBox->setContentsMargins(0, 6, 0, 2);
        detailVBox->setSpacing(4);

        auto* sep = new QFrame(detailWidget);
        sep->setFrameShape(QFrame::HLine);
        sep->setStyleSheet("color: rgba(255, 255, 255, 25); max-height: 1px;");
        detailVBox->addWidget(sep);

        auto* descText = new QLabel(a.description.isEmpty() ? "No additional details provided." : a.description, detailWidget);
        descText->setStyleSheet("color: #e2e8f0; font-size: 11px; line-height: 140%;");
        descText->setWordWrap(true);
        detailVBox->addWidget(descText);

        auto* metaHBox = new QHBoxLayout;
        metaHBox->setSpacing(8);

        auto* repLbl = new QLabel(QString("By: %1").arg(a.reporter.isEmpty() ? "Staff" : a.reporter), detailWidget);
        repLbl->setStyleSheet("color: #94a3b8; font-size: 10px; font-weight: 600;");
        metaHBox->addWidget(repLbl);

        metaHBox->addStretch(1);

        auto* statLbl = new QLabel(QString("Status: %1").arg(a.status.isEmpty() ? "Active" : a.status), detailWidget);
        statLbl->setStyleSheet(QString(
            "color: %1; font-size: 10px; font-weight: 700; background: rgba(255,255,255,10); border-radius: 4px; padding: 1px 6px;"
        ).arg(isAmbulance ? "#38bdf8" : "#fca5a5"));
        metaHBox->addWidget(statLbl);

        detailVBox->addLayout(metaHBox);
        cardVBox->addWidget(detailWidget);

        // Toggle action on button click
        auto* actionBtn = new QPushButton("Read Bulletin Details", alertCard);
        actionBtn->setCursor(Qt::PointingHandCursor);
        actionBtn->setStyleSheet(
            "QPushButton {"
            "  background: rgba(30, 41, 59, 180); color: #38bdf8; border: 1px solid rgba(56, 189, 248, 60);"
            "  border-radius: 6px; font-size: 10px; font-weight: 700; padding: 4px 8px;"
            "}"
            "QPushButton:hover {"
            "  background: rgba(56, 189, 248, 40); border: 1px solid #38bdf8;"
            "}"
        );

        const int idx = i;
        auto toggleCard = [this, idx, detailWidget, hintText, toggleIndicator, actionBtn]() {
            m_alerts[idx].expanded = !m_alerts[idx].expanded;
            const bool exp = m_alerts[idx].expanded;
            detailWidget->setVisible(exp);
            hintText->setText(exp ? QString::fromUtf8("Click to collapse \xE2\x96\xB4") : QString::fromUtf8("Click to read full bulletin \xE2\x96\xBE"));
            toggleIndicator->setText(exp ? QString::fromUtf8("\xE2\x96\xB2") : QString::fromUtf8("\xE2\x96\xBC"));
            actionBtn->setText(exp ? "Collapse Details" : "Read Bulletin Details");
            m_alertPanel->adjustSize();
            repositionAlertPanel();
        };

        connect(actionBtn, &QPushButton::clicked, this, toggleCard);
        cardVBox->addWidget(actionBtn);

        m_alertLayout->addWidget(alertCard);
    }

    m_alertLayout->addStretch(1);
    m_alertScroll->setWidget(m_alertContent);
    m_alertScroll->setMaximumHeight(480);
    panelLayout->addWidget(m_alertScroll, 1);

    m_alertPanel->adjustSize();

    // In StageIntro, keep hidden until login screen appears
    if (m_stage == StageLogin) {
        m_alertPanel->show();
        m_alertPanel->raise();
    } else {
        m_alertPanel->hide();
    }

    repositionAlertPanel();
}

void LoginWindow::repositionAlertPanel()
{
    if (!m_alertPanel) return;
    m_alertPanel->move(width() - m_alertPanel->width() - 20, 20);
    m_alertPanel->raise();
}

void LoginWindow::setupUI()
{
    auto* outer = new QVBoxLayout(this);
    outer->setAlignment(Qt::AlignCenter);

    m_cardWidget = new QWidget(this);
    auto* cardOuter = new QVBoxLayout(m_cardWidget);
    cardOuter->setContentsMargins(0, 0, 0, 0);

    auto* card = new QFrame;
    card->setObjectName("card");
    card->setFixedWidth(470);
    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(0, 0, 0, 0);
    cardLayout->setSpacing(0);

    // ── Header with University Logo prominently placed ahead of the name ──
    auto* header = new QFrame;
    header->setObjectName("cardHeader");
    auto* hl = new QHBoxLayout(header);
    hl->setContentsMargins(22, 18, 22, 18);
    hl->setSpacing(14);

    // 1. University Crest Logo
    auto* crestLabel = new QLabel;
    crestLabel->setPixmap(getUniversityLogo(56));
    crestLabel->setFixedSize(56, 56);
    crestLabel->setAlignment(Qt::AlignCenter);
    hl->addWidget(crestLabel);

    // 2. University Name & Title
    auto* titleBox = new QVBoxLayout;
    titleBox->setSpacing(2);
    titleBox->setContentsMargins(0, 0, 0, 0);

    auto* uniLogo = new QLabel("GRAPHIC ERA HILL UNIVERSITY");
    uniLogo->setObjectName("logoText");
    titleBox->addWidget(uniLogo);

    auto* subLogo = new QLabel(QString::fromUtf8("\xF0\x9F\x9B\xA1  CAMPUSCARE EMERGENCY RESPONSE"));
    subLogo->setObjectName("subLogo");
    titleBox->addWidget(subLogo);

    hl->addLayout(titleBox, 1);
    cardLayout->addWidget(header);

    // ── Body ─────────────────────────────────────────────────
    auto* body = new QFrame;
    body->setObjectName("cardBody");
    auto* bl = new QVBoxLayout(body);
    bl->setContentsMargins(32, 26, 32, 28);
    bl->setSpacing(12);

    bl->addWidget(makeRow(QString::fromUtf8("\xF0\x9F\x91\xA4"), m_idEdit, "Enter Student / Staff / Admin ID", false));

    // Dynamic Live Role Badge
    m_roleBadge = new QLabel(this);
    m_roleBadge->setObjectName("roleBadge");
    m_roleBadge->setAlignment(Qt::AlignCenter);
    m_roleBadge->hide();
    bl->addWidget(m_roleBadge);

    // Password row (revealed for admin)
    m_passRow = makeRow(QString::fromUtf8("\xF0\x9F\x94\x91"), m_passEdit, "Admin Password", true);
    m_passRow->setVisible(false);
    bl->addWidget(m_passRow);

    // Login Button
    m_loginBtn = new QPushButton(QString::fromUtf8("SECURE LOGIN  \xE2\x86\x92"));
    m_loginBtn->setObjectName("loginBtn");
    m_loginBtn->setCursor(Qt::PointingHandCursor);
    m_loginBtn->setFixedHeight(48);
    bl->addWidget(m_loginBtn);

    // Error Box
    m_error = new QLabel;
    m_error->setObjectName("errorLabel");
    m_error->setWordWrap(true);
    m_error->setAlignment(Qt::AlignCenter);
    m_error->hide();
    bl->addWidget(m_error);

    // ── Demo Quick-Fill Chips ────────────────────────────────
    auto* quickBox = new QHBoxLayout;
    quickBox->setSpacing(6);
    auto* quickLbl = new QLabel("Quick fill:");
    quickLbl->setStyleSheet("color: #64748b; font-size: 11px; font-weight: 500; background: transparent;");
    quickBox->addWidget(quickLbl);

    auto makeChip = [this, quickBox](const QString& label, const QString& id, const QString& pass) {
        auto* btn = new QPushButton(label);
        btn->setObjectName("quickChip");
        btn->setCursor(Qt::PointingHandCursor);
        connect(btn, &QPushButton::clicked, this, [this, id, pass]() {
            fillDemoAccount(id, pass);
        });
        quickBox->addWidget(btn);
    };

    makeChip("Student", "25101211029", "");
    makeChip("Faculty", "pleaseletmelogin", "");
    makeChip("Admin", "admin", "admin123");
    quickBox->addStretch(1);

    bl->addSpacing(4);
    bl->addLayout(quickBox);

    cardLayout->addWidget(body);
    cardOuter->addWidget(card);
    outer->addWidget(m_cardWidget);

    connect(m_idEdit, &QLineEdit::textChanged, this, &LoginWindow::onIdChanged);
    connect(m_idEdit, &QLineEdit::returnPressed, this, [this] {
        if (m_passRow->isVisible()) m_passEdit->setFocus();
        else attemptLogin();
    });
    connect(m_passEdit, &QLineEdit::returnPressed, this, &LoginWindow::attemptLogin);
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
}

QWidget* LoginWindow::makeRow(const QString& icon, QLineEdit*& edit,
                              const QString& placeholder, bool eye)
{
    auto* row = new QWidget;
    auto* h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    auto* ic = new QLabel(icon);
    ic->setObjectName("fieldIcon");
    ic->setAlignment(Qt::AlignCenter);
    ic->setFixedSize(52, 48);

    auto* field = new QFrame;
    field->setObjectName("field");
    field->setFixedHeight(48);
    auto* fl = new QHBoxLayout(field);
    fl->setContentsMargins(14, 0, 8, 0);
    fl->setSpacing(0);

    edit = new QLineEdit;
    edit->setPlaceholderText(placeholder);
    fl->addWidget(edit);

    if (eye) {
        edit->setEchoMode(QLineEdit::Password);
        auto* b = new QPushButton(QString::fromUtf8("\xF0\x9F\x91\x81"));
        b->setObjectName("eyeBtn");
        b->setCheckable(true);
        b->setCursor(Qt::PointingHandCursor);
        connect(b, &QPushButton::toggled, this, [edit](bool on) {
            edit->setEchoMode(on ? QLineEdit::Normal : QLineEdit::Password);
        });
        fl->addWidget(b);
    }

    h->addWidget(ic);
    h->addWidget(field, 1);
    return row;
}

void LoginWindow::fillDemoAccount(const QString& id, const QString& pass)
{
    m_idEdit->setText(id);
    if (!pass.isEmpty() && m_passRow->isVisible()) {
        m_passEdit->setText(pass);
    }
}

void LoginWindow::setIntroProgress(double val)
{
    m_introProgress = val;
    update();
}

void LoginWindow::setCardAnimProgress(double val)
{
    m_cardAnimProgress = val;
    update();
}

void LoginWindow::skipIntro()
{
    if (m_stage == StageIntro) {
        m_stage = StageLogin;
        m_skipBtn->hide();
        m_cardWidget->show();

        if (m_alertPanel) {
            m_alertPanel->show();
            m_alertPanel->raise();
            repositionAlertPanel();
        }

        // Smooth pop-in animation
        auto* anim = new QPropertyAnimation(this, "cardAnimProgress", this);
        anim->setDuration(400);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        anim->start(QAbstractAnimation::DeleteWhenStopped);

        m_idEdit->setFocus();
        update();
    }
}

void LoginWindow::onBgTimerTick()
{
    // Silky, gentle continuous floating drift (continuous sine, NO periodic jumps)
    m_bgPanAngle += 0.003;
    m_introPulseTick++;

    // Mouse parallax easing
    m_mouseOffset += (m_targetMouseOffset - m_mouseOffset) * 0.05;

    // Advance executive intro progress (~1.6 seconds total)
    if (m_stage == StageIntro) {
        m_introProgress += 0.022;
        if (m_introProgress >= 1.0) {
            skipIntro();
            return;
        }
    }

    update();
}

void LoginWindow::onIdChanged(const QString& text)
{
    m_error->hide();
    updateRoleBadge(text.trimmed());
}

void LoginWindow::updateRoleBadge(const QString& text)
{
    if (text.isEmpty()) {
        m_roleBadge->hide();
        m_passRow->setVisible(false);
        return;
    }

    QString role, pw;
    if (lookupUser(text, role, pw)) {
        m_roleBadge->show();
        if (role == "admin") {
            m_roleBadge->setText(QString::fromUtf8("\xF0\x9F\x9B\xA1  Administrator Account (Password Required)"));
            m_roleBadge->setStyleSheet("background: rgba(139, 92, 246, 35); color: #c084fc; border: 1px solid rgba(139, 92, 246, 80);");
            m_passRow->setVisible(true);
        } else if (role == "faculty") {
            m_roleBadge->setText(QString::fromUtf8("\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x8F\xAB  Faculty Member (Direct Access)"));
            m_roleBadge->setStyleSheet("background: rgba(16, 185, 129, 35); color: #34d399; border: 1px solid rgba(16, 185, 129, 80);");
            m_passRow->setVisible(false);
        } else {
            m_roleBadge->setText(QString::fromUtf8("\xF0\x9F\x8E\x93  Verified Student ID (Direct Access)"));
            m_roleBadge->setStyleSheet("background: rgba(2, 136, 209, 35); color: #38bdf8; border: 1px solid rgba(2, 136, 209, 80);");
            m_passRow->setVisible(false);
        }
    } else {
        m_roleBadge->show();
        m_roleBadge->setText(QString::fromUtf8("Unregistered ID (Check credentials)"));
        m_roleBadge->setStyleSheet("background: rgba(100, 116, 139, 30); color: #94a3b8; border: 1px solid rgba(100, 116, 139, 60);");
        m_passRow->setVisible(false);
    }
}

void LoginWindow::attemptLogin()
{
    const QString id = m_idEdit->text().trimmed();
    if (id.isEmpty()) { showError("Please enter your User ID."); return; }

    QString role, pw;
    if (!lookupUser(id, role, pw)) {
        showError("ID not recognized. Check users.txt or use quick-fill below.");
        return;
    }

    if (role == "admin" && m_passEdit->text() != pw) {
        showError("Incorrect admin password.");
        return;
    }

    emit loggedIn(id, role);
}

void LoginWindow::showError(const QString& msg)
{
    m_error->setText(msg);
    m_error->show();
}

void LoginWindow::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);

    // Pre-scale background image ONCE on resize with generous 80px overscan margin
    // This completely eliminates any real-time scaling jitter or shaking!
    if (!m_bg.isNull()) {
        const QSize targetSize = size() + QSize(100, 100);
        m_scaledBg = m_bg.scaled(targetSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }

    if (m_skipBtn) {
        m_skipBtn->move(width() - m_skipBtn->width() - 24, height() - m_skipBtn->height() - 20);
    }

    repositionAlertPanel();
}

void LoginWindow::mouseMoveEvent(QMouseEvent* event)
{
    // Smooth target mouse offset for subtle parallax
    const double nx = (event->pos().x() - width() / 2.0) / (width() / 2.0);
    const double ny = (event->pos().y() - height() / 2.0) / (height() / 2.0);
    m_targetMouseOffset = QPointF(nx * 12.0, ny * 10.0);
}

void LoginWindow::mousePressEvent(QMouseEvent* event)
{
    if (m_stage == StageIntro) {
        skipIntro();
        return;
    }
    QWidget::mousePressEvent(event);
}

void LoginWindow::keyPressEvent(QKeyEvent* event)
{
    if (m_stage == StageIntro) {
        skipIntro();
        return;
    }
    QWidget::keyPressEvent(event);
}

void LoginWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    // ── 1. Smooth, Stable Floating Background (NO real-time scaling) ──
    if (!m_scaledBg.isNull()) {
        const double panX = std::sin(m_bgPanAngle) * 18.0 + m_mouseOffset.x();
        const double panY = std::cos(m_bgPanAngle * 0.7) * 12.0 + m_mouseOffset.y();

        const QPointF drawPos(
            (width() - m_scaledBg.width()) / 2.0 + panX,
            (height() - m_scaledBg.height()) / 2.0 + panY
        );
        p.drawPixmap(drawPos, m_scaledBg);
    }

    // ── 2. Cinematic Vignette Gradient Overlay ───────────────
    QRadialGradient grad(width() / 2.0, height() / 2.0, std::max(width(), height()) * 0.7);
    grad.setColorAt(0.0, QColor(10, 15, 26, 175));
    grad.setColorAt(0.65, QColor(10, 15, 26, 215));
    grad.setColorAt(1.0, QColor(8, 11, 20, 245));
    p.fillRect(rect(), grad);

    // ── 3. Stage 1: Professional Institutional Portal Splash ──
    if (m_stage == StageIntro) {
        const double cx = width() / 2.0;
        const double cy = height() / 2.0 - 25;

        // University Crest in Center
        const int crestSize = 82;
        QPixmap crest = getUniversityLogo(crestSize);
        p.drawPixmap(cx - crestSize / 2.0, cy - crestSize / 2.0 - 10, crest);

        // University Name & Portal Header
        p.setFont(QFont("Segoe UI", 16, QFont::Bold));
        p.setPen(QColor("#f8fafc"));
        p.drawText(QRectF(cx - 300, cy + 45, 600, 26), Qt::AlignCenter, "GRAPHIC ERA HILL UNIVERSITY");

        p.setFont(QFont("Segoe UI", 11, QFont::Bold));
        p.setPen(QColor("#00e5ff"));
        p.drawText(QRectF(cx - 300, cy + 73, 600, 20), Qt::AlignCenter,
                   QString::fromUtf8("CAMPUSCARE  \xE2\x80\xA2  SAFETY & EMERGENCY PORTAL"));

        // Status string
        QString statusMsg = "Authenticating Terminal...";
        if (m_introProgress > 0.35) statusMsg = "Loading Campus Nodes & Fleet Telemetry...";
        if (m_introProgress > 0.75) statusMsg = "Portal Ready. Loading Access Credentials...";

        p.setFont(QFont("Segoe UI", 9, QFont::Medium));
        p.setPen(QColor("#94a3b8"));
        p.drawText(QRectF(cx - 200, cy + 115, 400, 18), Qt::AlignCenter, statusMsg);

        // Sleek Minimalist Progress Bar
        const double barW = 220.0;
        const double barH = 3.0;
        const double barX = cx - barW / 2.0;
        const double barY = cy + 140;

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(30, 41, 59, 200));
        p.drawRoundedRect(QRectF(barX, barY, barW, barH), 1.5, 1.5);

        QLinearGradient progGrad(barX, barY, barX + barW, barY);
        progGrad.setColorAt(0.0, QColor("#0288d1"));
        progGrad.setColorAt(1.0, QColor("#00e5ff"));
        p.setBrush(progGrad);
        p.drawRoundedRect(QRectF(barX, barY, barW * std::min(1.0, m_introProgress), barH), 1.5, 1.5);
    }
}
