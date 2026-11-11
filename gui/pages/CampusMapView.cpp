#include "CampusMapView.h"
#include "../theme/ThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>

CampusMapView::CampusMapView(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(420);

    // Normalized coordinates (0.0 to 1.0)
    m_coords["New Audi"]       = QPointF(0.08, 0.12);
    m_coords["Fashion dept"]   = QPointF(0.24, 0.12);
    m_coords["CSE dept"]       = QPointF(0.40, 0.12);
    m_coords["Open Audi"]      = QPointF(0.40, 0.38);
    m_coords["Gehu ground"]    = QPointF(0.24, 0.38);
    m_coords["Basement"]       = QPointF(0.24, 0.62);
    m_coords["Cafeteria"]      = QPointF(0.56, 0.38);
    m_coords["Gate 1"]         = QPointF(0.56, 0.62);
    m_coords["Tuck shop"]      = QPointF(0.40, 0.62);
    m_coords["Mechanical lab"] = QPointF(0.08, 0.62);
    m_coords["Civil lab"]      = QPointF(0.08, 0.86);
    m_coords["Girls hostel"]   = QPointF(0.40, 0.86);
    m_coords["Main gate"]      = QPointF(0.56, 0.86);
    m_coords["Tyagi Clinic"]   = QPointF(0.85, 0.62);
    m_coords["Hospital"]       = QPointF(0.85, 0.86);
}

void CampusMapView::setHighlightedPath(const QStringList& path)
{
    m_highlightedPath = path;
    update();
}

void CampusMapView::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const bool isDark = ThemeManager::instance().currentTheme() == ThemeManager::Dark;

    const QColor bgCol        = isDark ? QColor("#12171f") : QColor("#ffffff");
    const QColor edgeCol      = isDark ? QColor("#2a3545") : QColor("#d0d6de");
    const QColor highlightCol = isDark ? QColor("#00e5ff") : QColor("#0288d1");
    const QColor nodeCol      = isDark ? QColor("#1a222e") : QColor("#f3f6f9");
    const QColor nodeBorder   = isDark ? QColor("#404e63") : QColor("#a6b4c4");
    const QColor textCol      = isDark ? QColor("#e8edf3") : QColor("#1d2733");
    const QColor mutedTextCol = isDark ? QColor("#7d8c9e") : QColor("#667382");

    p.fillRect(rect(), bgCol);

    const int pad = 40;
    const double w = width() - 2 * pad;
    const double h = height() - 2 * pad;

    auto toScreen = [&](const QPointF& norm) -> QPointF {
        return QPointF(pad + norm.x() * w, pad + norm.y() * h);
    };

    auto isEdgeInPath = [&](const QString& u, const QString& v) -> bool {
        if (m_highlightedPath.size() < 2) return false;
        for (int i = 0; i < m_highlightedPath.size() - 1; ++i) {
            if ((m_highlightedPath[i] == u && m_highlightedPath[i+1] == v) ||
                (m_highlightedPath[i] == v && m_highlightedPath[i+1] == u)) {
                return true;
            }
        }
        return false;
    };

    const auto edges = RouteService::getEdges();

    // 1. Draw regular edges
    for (const auto& e : edges) {
        if (isEdgeInPath(e.u, e.v)) continue;
        QPointF p1 = toScreen(m_coords[e.u]);
        QPointF p2 = toScreen(m_coords[e.v]);

        p.setPen(QPen(edgeCol, 1.5));
        p.drawLine(p1, p2);

        // Edge weight text
        QPointF mid = (p1 + p2) / 2.0;
        p.setFont(QFont("Segoe UI", 8));
        p.setPen(mutedTextCol);
        p.drawText(QRectF(mid.x() - 20, mid.y() - 10, 40, 20), Qt::AlignCenter, QString::number(e.weight, 'f', e.weight == (int)e.weight ? 0 : 1) + "m");
    }

    // 2. Draw highlighted path edges
    for (const auto& e : edges) {
        if (!isEdgeInPath(e.u, e.v)) continue;
        QPointF p1 = toScreen(m_coords[e.u]);
        QPointF p2 = toScreen(m_coords[e.v]);

        p.setPen(QPen(highlightCol, 3.5));
        p.drawLine(p1, p2);

        QPointF mid = (p1 + p2) / 2.0;
        p.setFont(QFont("Segoe UI", 9, QFont::Bold));
        p.setPen(highlightCol);
        p.drawText(QRectF(mid.x() - 25, mid.y() - 12, 50, 24), Qt::AlignCenter, QString::number(e.weight, 'f', e.weight == (int)e.weight ? 0 : 1) + "m");
    }

    // 3. Draw nodes
    const double radius = 13.0;
    const auto nodes = RouteService::getNodes();

    for (const auto& name : nodes) {
        QPointF pt = toScreen(m_coords[name]);
        const bool isInPath = m_highlightedPath.contains(name);
        const bool isStart = !m_highlightedPath.isEmpty() && m_highlightedPath.first() == name;
        const bool isEnd = !m_highlightedPath.isEmpty() && m_highlightedPath.last() == name;

        if (isStart) {
            p.setBrush(QColor("#00e676"));
            p.setPen(QPen(QColor("#00b248"), 2.0));
        } else if (isEnd) {
            p.setBrush(QColor("#ff1744"));
            p.setPen(QPen(QColor("#c4001d"), 2.0));
        } else if (isInPath) {
            p.setBrush(highlightCol);
            p.setPen(QPen(highlightCol.darker(120), 2.0));
        } else {
            p.setBrush(nodeCol);
            p.setPen(QPen(nodeBorder, 1.5));
        }

        p.drawEllipse(pt, radius, radius);

        // Label
        p.setFont(QFont("Segoe UI", 9, (isInPath ? QFont::Bold : QFont::Normal)));
        p.setPen(isInPath ? (isDark ? QColor("#ffffff") : QColor("#000000")) : textCol);

        QRectF textRect(pt.x() - 60, pt.y() + radius + 2, 120, 24);
        p.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, name);
    }
}

