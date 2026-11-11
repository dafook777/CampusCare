#include "MapPage.h"
#include "CampusMapView.h"
#include "../services/RouteService.h"
#include "../widgets/Badge.h"

#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MapPage::MapPage(QWidget* parent) : BasePage(parent)
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(14);

    // ── Controls Header Card ─────────────────────────────────
    auto* controlsCard = new QFrame(this);
    controlsCard->setObjectName("card");
    auto* ctrlLayout = new QHBoxLayout(controlsCard);
    ctrlLayout->setContentsMargins(16, 14, 16, 14);
    ctrlLayout->setSpacing(12);

    auto makeLabel = [](const QString& text) {
        auto* l = new QLabel(text);
        l->setStyleSheet("font-weight: 600; font-size: 13px;");
        return l;
    };

    ctrlLayout->addWidget(makeLabel("From:"));
    m_fromCombo = new QComboBox(controlsCard);
    m_fromCombo->addItems(RouteService::getNodes());
    m_fromCombo->setCurrentText("Main gate");
    m_fromCombo->setMinimumWidth(130);
    ctrlLayout->addWidget(m_fromCombo);

    ctrlLayout->addWidget(makeLabel("To:"));
    m_toCombo = new QComboBox(controlsCard);
    m_toCombo->addItems(RouteService::getNodes());
    m_toCombo->setCurrentText("CSE dept");
    m_toCombo->setMinimumWidth(130);
    ctrlLayout->addWidget(m_toCombo);

    ctrlLayout->addWidget(makeLabel("Routing:"));
    m_algoCombo = new QComboBox(controlsCard);
    m_algoCombo->addItem("Shortest distance (Dijkstra)", 0);
    m_algoCombo->addItem("Fewest hops (BFS)", 1);
    ctrlLayout->addWidget(m_algoCombo);

    m_findBtn = new QPushButton(QString::fromUtf8("\xF0\x9F\x9A\xA6  Find Route"), controlsCard);
    m_findBtn->setCursor(Qt::PointingHandCursor);
    m_findBtn->setFixedHeight(36);
    connect(m_findBtn, &QPushButton::clicked, this, &MapPage::onFindRoute);
    ctrlLayout->addWidget(m_findBtn);

    m_emergBtn = new QPushButton(QString::fromUtf8("\xF0\x9F\x8F\xA5  Nearest Emergency Facility"), controlsCard);
    m_emergBtn->setCursor(Qt::PointingHandCursor);
    m_emergBtn->setFixedHeight(36);
    m_emergBtn->setStyleSheet("background: #d32f2f; color: white; font-weight: 600;");
    connect(m_emergBtn, &QPushButton::clicked, this, &MapPage::onFindEmergency);
    ctrlLayout->addWidget(m_emergBtn);

    ctrlLayout->addStretch(1);
    mainLayout->addWidget(controlsCard);

    // ── Result Card ──────────────────────────────────────────
    auto* resultCard = new QFrame(this);
    resultCard->setObjectName("card");
    auto* resLayout = new QVBoxLayout(resultCard);
    resLayout->setContentsMargins(16, 12, 16, 12);
    resLayout->setSpacing(6);

    auto* pathRow = new QHBoxLayout;
    auto* pathTitle = new QLabel("<b>Computed Route:</b> ");
    pathTitle->setStyleSheet("font-size: 13px;");
    pathRow->addWidget(pathTitle);

    m_resultPath = new QLabel("Select nodes and click Find Route.");
    m_resultPath->setStyleSheet("font-size: 14px; font-weight: 600; color: #03a9f4;");
    m_resultPath->setWordWrap(true);
    pathRow->addWidget(m_resultPath, 1);
    resLayout->addLayout(pathRow);

    m_resultStats = new QLabel("Distance: \u2014 | Estimated Walk: \u2014");
    m_resultStats->setStyleSheet("font-size: 13px; color: #8b97a7;");
    resLayout->addWidget(m_resultStats);

    mainLayout->addWidget(resultCard);

    // ── Map View ─────────────────────────────────────────────
    auto* mapFrame = new QFrame(this);
    mapFrame->setObjectName("card");
    auto* mapLayout = new QVBoxLayout(mapFrame);
    mapLayout->setContentsMargins(4, 4, 4, 4);

    m_mapView = new CampusMapView(mapFrame);
    mapLayout->addWidget(m_mapView, 1);
    mainLayout->addWidget(mapFrame, 1);

    // Default route on load
    onFindRoute();
}

void MapPage::refresh()
{
    onFindRoute();
}

void MapPage::onFindRoute()
{
    const QString from = m_fromCombo->currentText();
    const QString to = m_toCombo->currentText();
    const int algo = m_algoCombo->currentData().toInt();

    RouteResult res = (algo == 0)
        ? RouteService::findShortestDistance(from, to)
        : RouteService::findFewestHops(from, to);

    if (!res.found) {
        m_resultPath->setText("No route found between selected points.");
        m_resultStats->setText("");
        m_mapView->setHighlightedPath({});
        return;
    }

    m_resultPath->setText(res.path.join(QString::fromUtf8("  \xE2\x86\x92  ")));
    m_resultStats->setText(QString("Total Distance: <b>%1 m</b> &nbsp;&nbsp;|&nbsp;&nbsp; Estimated Walking Time: <b>%2 min</b> (at 80 m/min)")
                           .arg(res.totalMetres, 0, 'f', 1)
                           .arg(res.walkingMinutes, 0, 'f', 1));
    m_mapView->setHighlightedPath(res.path);
}

void MapPage::onFindEmergency()
{
    const QString from = m_fromCombo->currentText();
    RouteResult res = RouteService::findNearestEmergency(from);

    if (!res.found || res.path.isEmpty()) {
        m_resultPath->setText("No route to emergency facility found.");
        return;
    }

    const QString dest = res.path.last();
    m_toCombo->setCurrentText(dest);

    m_resultPath->setText(QString::fromUtf8("\xF0\x9F\x9A\xA8 Nearest: %1 \u2014 ").arg(dest) + res.path.join(QString::fromUtf8("  \xE2\x86\x92  ")));
    m_resultStats->setText(QString("Total Distance: <b>%1 m</b> &nbsp;&nbsp;|&nbsp;&nbsp; Estimated Walking Time: <b>%2 min</b>")
                           .arg(res.totalMetres, 0, 'f', 1)
                           .arg(res.walkingMinutes, 0, 'f', 1));
    m_mapView->setHighlightedPath(res.path);
}

