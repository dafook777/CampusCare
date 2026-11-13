#include "CampusMapView.h"
#include "../theme/ThemeManager.h"
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <cmath>
#include <algorithm>

CampusMapView::CampusMapView(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(460);
    setMouseTracking(true);

    // ── 2D Normalized Coordinates (0.0 to 1.0) ───────────────
    m_coords2D["New Audi"]       = QPointF(0.08, 0.12);
    m_coords2D["Fashion dept"]   = QPointF(0.24, 0.12);
    m_coords2D["CSE dept"]       = QPointF(0.40, 0.12);
    m_coords2D["Open Audi"]      = QPointF(0.40, 0.38);
    m_coords2D["Gehu ground"]    = QPointF(0.24, 0.38);
    m_coords2D["Basement"]       = QPointF(0.24, 0.62);
    m_coords2D["Cafeteria"]      = QPointF(0.56, 0.38);
    m_coords2D["Gate 1"]         = QPointF(0.56, 0.62);
    m_coords2D["Tuck shop"]      = QPointF(0.40, 0.62);
    m_coords2D["Mechanical lab"] = QPointF(0.08, 0.62);
    m_coords2D["Civil lab"]      = QPointF(0.08, 0.86);
    m_coords2D["Girls hostel"]   = QPointF(0.40, 0.86);
    m_coords2D["Main gate"]      = QPointF(0.56, 0.86);
    m_coords2D["Tyagi Clinic"]   = QPointF(0.85, 0.62);
    m_coords2D["Hospital"]       = QPointF(0.85, 0.86);

    // ── 3D Campus Coordinates (X, Y in meters, Z is floor elevation) ──
    // Main Academic Building sits around Y = -90 to -30
    // GEHU Ground sits directly in front around Y = 0 to 80
    // Gates sit around Y = 130 to 180
    m_coords3D["CSE dept"]       = { 30.0, -60.0, 105.0,  3, "academic" }; // Floor 3
    m_coords3D["Fashion dept"]   = {-40.0, -60.0,  70.0,  2, "academic" }; // Floor 2
    m_coords3D["New Audi"]       = {-90.0, -60.0,  35.0,  1, "academic" }; // Floor 1
    m_coords3D["Mechanical lab"] = {-90.0, -25.0,  35.0,  1, "academic" }; // Floor 1 wing
    m_coords3D["Civil lab"]      = {-40.0, -25.0,  35.0,  1, "academic" }; // Floor 1 wing
    m_coords3D["Open Audi"]      = {  0.0, -30.0,   0.0,  0, "academic" }; // Ground Foyer
    m_coords3D["Cafeteria"]      = { 60.0, -30.0,   0.0,  0, "academic" }; // Ground East
    m_coords3D["Basement"]       = {-40.0, -45.0, -35.0, -1, "basement" }; // Sub-level

    // Campus Outdoor & Separate Facilities
    m_coords3D["Gehu ground"]    = {  0.0,  55.0,   0.0,  0, "field" };    // In front of building
    m_coords3D["Gate 1"]         = { 65.0, 135.0,   0.0,  0, "gate" };
    m_coords3D["Tuck shop"]      = {-45.0, 135.0,   0.0,  0, "kiosk" };
    m_coords3D["Girls hostel"]   = {-160.0, 75.0,  25.0,  1, "hostel" };   // Separate building
    m_coords3D["Main gate"]      = { 65.0, 195.0,   0.0,  0, "gate" };

    // Emergency Facilities
    m_coords3D["Tyagi Clinic"]   = {220.0,  95.0,   0.0,  0, "emerg" };
    m_coords3D["Hospital"]       = {235.0, 195.0,   0.0,  0, "emerg" };
}

void CampusMapView::setHighlightedPath(const QStringList& path)
{
    m_highlightedPath = path;
    update();
}

void CampusMapView::setViewMode(ViewMode mode)
{
    if (m_mode == mode) return;
    m_mode = mode;
    emit viewModeChanged(m_mode);
    update();
}

void CampusMapView::setFloorFilter(int floor)
{
    if (m_floorFilter == floor) return;
    m_floorFilter = floor;
    emit floorFilterChanged(m_floorFilter);
    update();
}

void CampusMapView::resetCamera()
{
    m_yaw = -0.55;
    m_pitch = 0.58;
    m_zoom = 1.0;
    m_floorFilter = -2;
    update();
}

void CampusMapView::zoomIn()
{
    m_zoom = std::min(2.5, m_zoom + 0.15);
    update();
}

void CampusMapView::zoomOut()
{
    m_zoom = std::max(0.5, m_zoom - 0.15);
    update();
}

// ── 3D Projection ───────────────────────────────────────────
QPointF CampusMapView::project3D(double x, double y, double z, const QPointF& origin, double scale) const
{
    const double cosY = std::cos(m_yaw);
    const double sinY = std::sin(m_yaw);
    const double cosP = std::cos(m_pitch);
    const double sinP = std::sin(m_pitch);

    // Yaw rotation around Z axis
    const double rx = x * cosY - y * sinY;
    const double ry = x * sinY + y * cosY;

    // Pitch tilt
    const double rz = z;
    const double px = rx;
    const double py = ry * sinP - rz * cosP;

    return QPointF(origin.x() + px * scale * m_zoom,
                   origin.y() + py * scale * m_zoom);
}

// ── Paint Event ─────────────────────────────────────────────
void CampusMapView::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (m_mode == Mode2D) {
        paint2D(p);
    } else {
        paint3D(p);
    }
}

// ── 2D Schematic Painter ────────────────────────────────────
void CampusMapView::paint2D(QPainter& p)
{
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
        QPointF p1 = toScreen(m_coords2D[e.u]);
        QPointF p2 = toScreen(m_coords2D[e.v]);

        p.setPen(QPen(edgeCol, 1.5));
        p.drawLine(p1, p2);

        QPointF mid = (p1 + p2) / 2.0;
        p.setFont(QFont("Segoe UI", 8));
        p.setPen(mutedTextCol);
        p.drawText(QRectF(mid.x() - 20, mid.y() - 10, 40, 20), Qt::AlignCenter,
                   QString::number(e.weight, 'f', e.weight == (int)e.weight ? 0 : 1) + "m");
    }

    // 2. Draw highlighted path
    for (const auto& e : edges) {
        if (!isEdgeInPath(e.u, e.v)) continue;
        QPointF p1 = toScreen(m_coords2D[e.u]);
        QPointF p2 = toScreen(m_coords2D[e.v]);

        p.setPen(QPen(highlightCol, 3.5));
        p.drawLine(p1, p2);

        QPointF mid = (p1 + p2) / 2.0;
        p.setFont(QFont("Segoe UI", 9, QFont::Bold));
        p.setPen(highlightCol);
        p.drawText(QRectF(mid.x() - 25, mid.y() - 12, 50, 24), Qt::AlignCenter,
                   QString::number(e.weight, 'f', e.weight == (int)e.weight ? 0 : 1) + "m");
    }

    // 3. Draw nodes
    const double radius = 13.0;
    const auto nodes = RouteService::getNodes();

    for (const auto& name : nodes) {
        QPointF pt = toScreen(m_coords2D[name]);
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

        p.setFont(QFont("Segoe UI", 9, (isInPath ? QFont::Bold : QFont::Normal)));
        p.setPen(isInPath ? (isDark ? QColor("#ffffff") : QColor("#000000")) : textCol);

        QRectF textRect(pt.x() - 60, pt.y() + radius + 2, 120, 24);
        p.drawText(textRect, Qt::AlignHCenter | Qt::AlignTop, name);
    }
}

// ── 3D Campus Tour Painter ──────────────────────────────────
void CampusMapView::paint3D(QPainter& p)
{
    const bool isDark = ThemeManager::instance().currentTheme() == ThemeManager::Dark;

    const QColor bgCol       = isDark ? QColor("#0e1319") : QColor("#f0f4f8");
    const QColor groundColor = isDark ? QColor("#141b24") : QColor("#e2e8f0");
    const QColor gridColor   = isDark ? QColor("#1c2533") : QColor("#cbd5e1");
    const QColor turfColor   = isDark ? QColor("#183824") : QColor("#86efac");
    const QColor turfBorder  = isDark ? QColor("#22c55e") : QColor("#16a34a");
    const QColor roadColor   = isDark ? QColor("#232e3d") : QColor("#cbd5e1");
    const QColor bldgBaseCol = isDark ? QColor("#18212e") : QColor("#f1f5f9");
    const QColor bldgWallCol = isDark ? QColor(30, 41, 59, 140) : QColor(203, 213, 225, 160);
    const QColor bldgEdgeCol = isDark ? QColor("#38bdf8") : QColor("#0288d1");
    const QColor highlightCol= isDark ? QColor("#00e5ff") : QColor("#0288d1");
    const QColor textCol     = isDark ? QColor("#e2e8f0") : QColor("#1e293b");

    p.fillRect(rect(), bgCol);

    const QPointF origin(width() / 2.0 + 10, height() / 2.0 + 30);
    const double scale = std::min(width(), height()) / 440.0;

    auto project = [&](double x, double y, double z) -> QPointF {
        return project3D(x, y, z, origin, scale);
    };

    // Helper: Draw 3D Box (cx, cy, w, d, z1, z2)
    auto drawBox = [&](double cx, double cy, double w, double d, double z1, double z2,
                       const QColor& topCol, const QColor& sideCol, const QColor& strokeCol) {
        const double x1 = cx - w/2.0, x2 = cx + w/2.0;
        const double y1 = cy - d/2.0, y2 = cy + d/2.0;

        QPointF b1 = project(x1, y1, z1);
        QPointF b2 = project(x2, y1, z1);
        QPointF b3 = project(x2, y2, z1);
        QPointF b4 = project(x1, y2, z1);

        QPointF t1 = project(x1, y1, z2);
        QPointF t2 = project(x2, y1, z2);
        QPointF t3 = project(x2, y2, z2);
        QPointF t4 = project(x1, y2, z2);

        // Right side
        QPolygonF rightSide;
        rightSide << b3 << b2 << t2 << t3;
        p.setBrush(sideCol.darker(115));
        p.setPen(QPen(strokeCol, 1.0));
        p.drawPolygon(rightSide);

        // Front side
        QPolygonF frontSide;
        frontSide << b4 << b3 << t3 << t4;
        p.setBrush(sideCol);
        p.setPen(QPen(strokeCol, 1.0));
        p.drawPolygon(frontSide);

        // Top face
        QPolygonF topFace;
        topFace << t1 << t2 << t3 << t4;
        p.setBrush(topCol);
        p.setPen(QPen(strokeCol, 1.2));
        p.drawPolygon(topFace);
    };

    // 1. Campus Ground Base Grid
    {
        const double sz = 300.0;
        QPolygonF basePlate;
        basePlate << project(-sz, -sz*0.8, 0)
                  << project( sz, -sz*0.8, 0)
                  << project( sz,  sz*0.9, 0)
                  << project(-sz,  sz*0.9, 0);
        p.setBrush(groundColor);
        p.setPen(QPen(gridColor, 1.0));
        p.drawPolygon(basePlate);
    }

    // 2. GEHU Ground (Lawn & Athletic Field directly in front of main building)
    if (m_floorFilter == -2 || m_floorFilter == 0) {
        const double gx = 0.0, gy = 55.0, gw = 200.0, gd = 100.0;
        QPolygonF turf;
        turf << project(gx - gw/2, gy - gd/2, 1)
             << project(gx + gw/2, gy - gd/2, 1)
             << project(gx + gw/2, gy + gd/2, 1)
             << project(gx - gw/2, gy + gd/2, 1);
        p.setBrush(turfColor);
        p.setPen(QPen(turfBorder, 2.0));
        p.drawPolygon(turf);

        // Field center circle & label
        QPointF fieldCenter = project(gx, gy, 2);
        p.setPen(QPen(QColor(255, 255, 255, 80), 1.5, Qt::DashLine));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(fieldCenter, 22 * m_zoom, 14 * m_zoom);

        p.setFont(QFont("Segoe UI", 9, QFont::Bold));
        p.setPen(turfBorder.lighter(130));
        p.drawText(QRectF(fieldCenter.x() - 60, fieldCenter.y() - 10, 120, 20),
                   Qt::AlignCenter, "GEHU GROUND");
    }

    // 3. Paved Pathways connecting landmarks
    {
        p.setPen(QPen(roadColor, 5.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        // Main Gate -> Gate 1
        p.drawLine(project(65, 195, 0.5), project(65, 135, 0.5));
        // Gate 1 -> Open Audi
        p.drawLine(project(65, 135, 0.5), project(0, -30, 0.5));
        // Gate 1 -> Tuck Shop
        p.drawLine(project(65, 135, 0.5), project(-45, 135, 0.5));
        // Tuck Shop -> Girls Hostel
        p.drawLine(project(-45, 135, 0.5), project(-160, 75, 0.5));
        // Gate 1 -> Tyagi Clinic
        p.drawLine(project(65, 135, 0.5), project(220, 95, 0.5));
        // Main Gate -> Hospital
        p.drawLine(project(65, 195, 0.5), project(235, 195, 0.5));
    }

    // 4. Girls Hostel (Separate Residential Building)
    if (m_floorFilter == -2 || m_floorFilter == 1 || m_floorFilter == 0) {
        drawBox(-160, 75, 80, 70, 0, 45,
                isDark ? QColor("#854d0e") : QColor("#fde047"),
                isDark ? QColor("#713f12") : QColor("#eab308"),
                isDark ? QColor("#ca8a04") : QColor("#a16207"));
        QPointF hostelLbl = project(-160, 75, 52);
        p.setFont(QFont("Segoe UI", 8, QFont::Bold));
        p.setPen(isDark ? QColor("#fef08a") : QColor("#713f12"));
        p.drawText(QRectF(hostelLbl.x() - 50, hostelLbl.y() - 8, 100, 16),
                   Qt::AlignCenter, "Girls Hostel");
    }

    // 5. Tuck Shop (Standalone Kiosk)
    if (m_floorFilter == -2 || m_floorFilter == 0) {
        drawBox(-45, 135, 26, 26, 0, 18,
                isDark ? QColor("#ea580c") : QColor("#fdba74"),
                isDark ? QColor("#c2410c") : QColor("#fb923c"),
                QColor("#9a3412"));
        QPointF tuckLbl = project(-45, 135, 24);
        p.setFont(QFont("Segoe UI", 8, QFont::Bold));
        p.setPen(textCol);
        p.drawText(QRectF(tuckLbl.x() - 40, tuckLbl.y() - 8, 80, 16),
                   Qt::AlignCenter, "Tuck Shop");
    }

    // 6. Main Gate & Gate 1 Entrance Portals
    if (m_floorFilter == -2 || m_floorFilter == 0) {
        // Gate 1 pillars
        drawBox(65, 135, 28, 8, 0, 16, QColor("#64748b"), QColor("#475569"), QColor("#334155"));
        // Main Gate arch
        drawBox(65, 195, 36, 10, 0, 22, QColor("#0288d1"), QColor("#0369a1"), QColor("#075985"));
    }

    // 7. Medical Facilities (Clinic & Hospital)
    if (m_floorFilter == -2 || m_floorFilter == 0) {
        // Clinic
        drawBox(220, 95, 45, 45, 0, 25,
                isDark ? QColor("#991b1b") : QColor("#fecaca"),
                isDark ? QColor("#7f1d1d") : QColor("#f87171"),
                QColor("#ef4444"));
        // Hospital
        drawBox(235, 195, 60, 50, 0, 40,
                isDark ? QColor("#991b1b") : QColor("#fecaca"),
                isDark ? QColor("#7f1d1d") : QColor("#f87171"),
                QColor("#dc2626"));
    }

    // 8. Main Academic Building (Multi-Story Structure)
    // Slabs: Basement (-35), Ground (0), 1F (35), 2F (70), 3F (105)
    const double mbX = -20.0, mbY = -60.0, mbW = 220.0, mbD = 110.0;

    auto drawFloorSlab = [&](double z, const QString& label, const QColor& tint) {
        QPolygonF slab;
        slab << project(mbX - mbW/2, mbY - mbD/2, z)
             << project(mbX + mbW/2, mbY - mbD/2, z)
             << project(mbX + mbW/2, mbY + mbD/2, z)
             << project(mbX - mbW/2, mbY + mbD/2, z);
        p.setBrush(tint);
        p.setPen(QPen(bldgEdgeCol, 1.5));
        p.drawPolygon(slab);

        // Floor label tag
        QPointF tagPt = project(mbX - mbW/2 + 10, mbY + mbD/2 - 5, z);
        p.setFont(QFont("Segoe UI", 8, QFont::Bold));
        p.setPen(bldgEdgeCol.lighter(130));
        p.drawText(QRectF(tagPt.x(), tagPt.y() - 10, 160, 16), Qt::AlignLeft, label);
    };

    // Basement Sunk Level
    if (m_floorFilter == -2 || m_floorFilter == -1) {
        drawFloorSlab(-35, "BASEMENT (Labs)",
                      isDark ? QColor(220, 38, 38, 35) : QColor(254, 202, 202, 60));
    }

    // Ground Floor
    if (m_floorFilter == -2 || m_floorFilter == 0) {
        drawFloorSlab(0, "GROUND FLOOR (Audi/Cafe)",
                      isDark ? QColor(30, 41, 59, 150) : QColor(241, 245, 249, 180));
    }

    // 1st Floor
    if (m_floorFilter == -2 || m_floorFilter == 1) {
        drawFloorSlab(35, "1F (New Audi / Mech / Civil)",
                      isDark ? QColor(14, 116, 144, 45) : QColor(186, 230, 253, 90));
    }

    // 2nd Floor
    if (m_floorFilter == -2 || m_floorFilter == 2) {
        drawFloorSlab(70, "2F (Fashion Dept)",
                      isDark ? QColor(147, 51, 234, 40) : QColor(233, 213, 255, 90));
    }

    // 3rd Floor
    if (m_floorFilter == -2 || m_floorFilter == 3) {
        drawFloorSlab(105, "3F (CSE Dept)",
                      isDark ? QColor(6, 182, 212, 65) : QColor(165, 243, 252, 120));
    }

    // Architectural Support Columns & Glass Facade
    if (m_floorFilter == -2) {
        const double xPoints[] = { mbX - mbW/2, mbX, mbX + mbW/2 };
        const double yPoints[] = { mbY - mbD/2, mbY + mbD/2 };
        p.setPen(QPen(isDark ? QColor(56, 189, 248, 80) : QColor(2, 136, 209, 80), 1.2));
        for (double xp : xPoints) {
            for (double yp : yPoints) {
                p.drawLine(project(xp, yp, 0), project(xp, yp, 105));
            }
        }

        // Visible Vertical Stairwell Column inside building
        const double sx = 30.0, sy = -30.0;
        QPolygonF stairwell;
        stairwell << project(sx - 12, sy - 12, 0)
                  << project(sx + 12, sy - 12, 0)
                  << project(sx + 12, sy + 12, 0)
                  << project(sx - 12, sy + 12, 0);
        p.setBrush(isDark ? QColor(6, 182, 212, 30) : QColor(2, 136, 209, 30));
        p.setPen(QPen(QColor("#00e5ff"), 1.0, Qt::DashLine));
        p.drawPolygon(stairwell);
        p.drawLine(project(sx, sy, 0), project(sx, sy, 105));

        QPointF stairTag = project(sx, sy, 115);
        p.setFont(QFont("Segoe UI", 7, QFont::Bold));
        p.setPen(QColor("#00e5ff"));
        p.drawText(QRectF(stairTag.x() - 40, stairTag.y() - 8, 80, 16),
                   Qt::AlignCenter, "Stairwell / Lift");
    }

    // 9. Highlighted Route Path in 3D
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

    // Draw inactive edges faintly
    for (const auto& e : edges) {
        if (!m_coords3D.contains(e.u) || !m_coords3D.contains(e.v)) continue;
        if (isEdgeInPath(e.u, e.v)) continue;

        const auto& nu = m_coords3D[e.u];
        const auto& nv = m_coords3D[e.v];
        if (m_floorFilter != -2 && nu.floor != m_floorFilter && nv.floor != m_floorFilter) continue;

        QPointF p1 = project(nu.x, nu.y, nu.z);
        QPointF p2 = project(nv.x, nv.y, nv.z);

        p.setPen(QPen(isDark ? QColor("#334155") : QColor("#94a3b8"), 1.2));
        p.drawLine(p1, p2);
    }

    // Draw active glowing path
    for (int i = 0; i < m_highlightedPath.size() - 1; ++i) {
        const QString u = m_highlightedPath[i];
        const QString v = m_highlightedPath[i+1];
        if (!m_coords3D.contains(u) || !m_coords3D.contains(v)) continue;

        const auto& nu = m_coords3D[u];
        const auto& nv = m_coords3D[v];

        QPointF p1 = project(nu.x, nu.y, nu.z);
        QPointF p2 = project(nv.x, nv.y, nv.z);

        // Path glow
        p.setPen(QPen(QColor(0, 229, 255, 70), 8.0, Qt::SolidLine, Qt::RoundCap));
        p.drawLine(p1, p2);

        // Path core
        p.setPen(QPen(QColor("#00e5ff"), 3.5, nu.z != nv.z ? Qt::DashLine : Qt::SolidLine, Qt::RoundCap));
        p.drawLine(p1, p2);

        if (nu.z != nv.z) {
            QPointF mid = (p1 + p2) / 2.0;
            p.setFont(QFont("Segoe UI", 8, QFont::Bold));
            p.setPen(QColor("#00e5ff"));
            p.drawText(QRectF(mid.x() + 5, mid.y() - 8, 120, 16), Qt::AlignLeft, "🪜 Stairs Climb");
        }
    }

    // 10. Draw 3D Location Nodes (Spheres & Labels)
    const auto nodes = RouteService::getNodes();
    for (const auto& name : nodes) {
        if (!m_coords3D.contains(name)) continue;
        const auto& n = m_coords3D[name];
        if (m_floorFilter != -2 && n.floor != m_floorFilter) continue;

        QPointF pt = project(n.x, n.y, n.z);
        const bool isInPath = m_highlightedPath.contains(name);
        const bool isStart = !m_highlightedPath.isEmpty() && m_highlightedPath.first() == name;
        const bool isEnd = !m_highlightedPath.isEmpty() && m_highlightedPath.last() == name;

        // Ground drop-pin pillar for elevated floors
        if (n.z != 0.0) {
            QPointF ptGnd = project(n.x, n.y, 0.0);
            p.setPen(QPen(n.z > 0 ? QColor(56, 189, 248, 100) : QColor(239, 68, 68, 100), 1.0, Qt::DashLine));
            p.drawLine(pt, ptGnd);
        }

        // Draw pin sphere
        const double r = (isStart || isEnd) ? 8.5 : (isInPath ? 7.0 : 5.5);
        p.setPen(Qt::NoPen);

        if (isStart) {
            p.setBrush(QColor("#00e676"));
            p.drawEllipse(pt, r + 4, r + 4);
            p.setBrush(QColor("#ffffff"));
            p.drawEllipse(pt, r, r);
        } else if (isEnd) {
            p.setBrush(QColor("#ff1744"));
            p.drawEllipse(pt, r + 4, r + 4);
            p.setBrush(QColor("#ffffff"));
            p.drawEllipse(pt, r, r);
        } else if (isInPath) {
            p.setBrush(QColor("#00e5ff"));
            p.drawEllipse(pt, r, r);
        } else if (n.category == "emerg") {
            p.setBrush(QColor("#ef4444"));
            p.drawEllipse(pt, r, r);
        } else if (n.floor == -1) {
            p.setBrush(QColor("#f59e0b"));
            p.drawEllipse(pt, r, r);
        } else {
            p.setBrush(isDark ? QColor("#94a3b8") : QColor("#475569"));
            p.drawEllipse(pt, r, r);
        }

        // Label
        p.setFont(QFont("Segoe UI", isInPath ? 9 : 8, isInPath ? QFont::Bold : QFont::Normal));
        p.setPen(isInPath ? (isDark ? QColor("#ffffff") : QColor("#000000")) : textCol);

        QRectF txtRect(pt.x() - 65, pt.y() + r + 2, 130, 20);
        p.drawText(txtRect, Qt::AlignCenter, name);
    }

    // ── 11. Canvas On-Screen Controls Overlay ─────────────────
    // Floor switcher pills at top-right
    const QStringList floorLabels = {"All", "3F", "2F", "1F", "Gnd", "Base"};
    const int floorVals[] = {-2, 3, 2, 1, 0, -1};
    int pillX = width() - 250;
    const int pillY = 12;

    p.setFont(QFont("Segoe UI", 8, QFont::Bold));
    m_floorBtnRects.clear();
    for (int i = 0; i < 6; ++i) {
        QRect btnRect(pillX, pillY, 36, 24);
        m_floorBtnRects[floorVals[i]] = btnRect;
        const bool active = (m_floorFilter == floorVals[i]);

        p.setBrush(active ? QColor("#03a9f4") : (isDark ? QColor("#1e293b") : QColor("#e2e8f0")));
        p.setPen(QPen(isDark ? QColor("#334155") : QColor("#cbd5e1"), 1.0));
        p.drawRoundedRect(btnRect, 4, 4);

        p.setPen(active ? QColor("#ffffff") : (isDark ? QColor("#cbd5e1") : QColor("#334155")));
        p.drawText(btnRect, Qt::AlignCenter, floorLabels[i]);
        pillX += 39;
    }

    // Reset View & Zoom buttons at bottom-right
    m_rectResetBtn = QRect(width() - 95, height() - 36, 85, 26);
    p.setBrush(isDark ? QColor("#1e293b") : QColor("#e2e8f0"));
    p.setPen(QPen(isDark ? QColor("#334155") : QColor("#cbd5e1"), 1.0));
    p.drawRoundedRect(m_rectResetBtn, 4, 4);
    p.setFont(QFont("Segoe UI", 8, QFont::DemiBold));
    p.setPen(textCol);
    p.drawText(m_rectResetBtn, Qt::AlignCenter, "Reset View");

    m_rectZoomIn  = QRect(width() - 65, height() - 70, 26, 26);
    m_rectZoomOut = QRect(width() - 34, height() - 70, 26, 26);

    p.drawRoundedRect(m_rectZoomIn, 4, 4);
    p.drawText(m_rectZoomIn, Qt::AlignCenter, "+");

    p.drawRoundedRect(m_rectZoomOut, 4, 4);
    p.drawText(m_rectZoomOut, Qt::AlignCenter, "-");

    // Camera Hint at bottom-left
    p.setFont(QFont("Segoe UI", 8));
    p.setPen(isDark ? QColor("#64748b") : QColor("#94a3b8"));
    p.drawText(QRect(14, height() - 28, 300, 20), Qt::AlignLeft | Qt::AlignVCenter,
               "👆 Drag to rotate 3D  |  Scroll to zoom");
}

// ── Interactive Mouse Handlers ──────────────────────────────
void CampusMapView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        const QPoint pos = event->pos();

        // Check Floor switcher clicks
        for (auto it = m_floorBtnRects.begin(); it != m_floorBtnRects.end(); ++it) {
            if (it.value().contains(pos)) {
                setFloorFilter(it.key());
                return;
            }
        }

        // Check Reset View click
        if (m_rectResetBtn.contains(pos)) {
            resetCamera();
            return;
        }

        // Check Zoom clicks
        if (m_rectZoomIn.contains(pos)) {
            zoomIn();
            return;
        }
        if (m_rectZoomOut.contains(pos)) {
            zoomOut();
            return;
        }

        // Orbit camera drag start
        m_isDragging = true;
        m_lastMousePos = pos;
        setCursor(Qt::ClosedHandCursor);
    }
}

void CampusMapView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        const QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();

        m_yaw += delta.x() * 0.008;
        m_pitch = std::max(0.15, std::min(1.25, m_pitch + delta.y() * 0.008));
        update();
        return;
    }

    // Cursor feedback over buttons
    const QPoint pos = event->pos();
    bool overButton = m_rectResetBtn.contains(pos) || m_rectZoomIn.contains(pos) || m_rectZoomOut.contains(pos);
    for (const auto& r : m_floorBtnRects) {
        if (r.contains(pos)) { overButton = true; break; }
    }
    setCursor(overButton ? Qt::PointingHandCursor : (m_mode == Mode3D ? Qt::OpenHandCursor : Qt::ArrowCursor));
}

void CampusMapView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void CampusMapView::wheelEvent(QWheelEvent* event)
{
    if (m_mode == Mode3D) {
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        event->accept();
    }
}
