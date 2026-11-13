#ifndef CAMPUSMAPVIEW_H
#define CAMPUSMAPVIEW_H

#include <QWidget>
#include <QStringList>
#include <QMap>
#include <QPointF>
#include "../services/RouteService.h"

class CampusMapView : public QWidget {
    Q_OBJECT
public:
    enum ViewMode {
        Mode2D,
        Mode3D
    };

    explicit CampusMapView(QWidget* parent = nullptr);

    void setHighlightedPath(const QStringList& path);
    void setViewMode(ViewMode mode);
    ViewMode viewMode() const { return m_mode; }

    void setFloorFilter(int floor); // -2 = all, -1 = basement, 0 = ground, 1 = 1F, 2 = 2F, 3 = 3F
    int floorFilter() const { return m_floorFilter; }

    void resetCamera();
    void zoomIn();
    void zoomOut();

signals:
    void viewModeChanged(ViewMode mode);
    void floorFilterChanged(int floor);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void paint2D(QPainter& p);
    void paint3D(QPainter& p);

    // 3D projection helper
    QPointF project3D(double x, double y, double z, const QPointF& origin, double scale) const;

    // 2D Normalized Coordinates (0.0 to 1.0)
    QMap<QString, QPointF> m_coords2D;

    // 3D Real-World Campus Node Coordinates (X: East-West, Y: North-South, Z: Height)
    struct Node3D {
        double x, y, z;
        int floor;
        QString category;
    };
    QMap<QString, Node3D> m_coords3D;

    QStringList m_highlightedPath;
    ViewMode    m_mode = Mode3D;
    int         m_floorFilter = -2; // -2 = All floors

    // 3D Interactive Camera
    double m_yaw   = -0.55;    // Horizontal rotation (radians)
    double m_pitch = 0.58;     // Vertical tilt (radians)
    double m_zoom  = 1.0;      // Zoom level
    QPoint m_lastMousePos;
    bool   m_isDragging = false;

    // Interactive button rects inside canvas
    QRect m_rectMode2D;
    QRect m_rectMode3D;
    QRect m_rectResetBtn;
    QRect m_rectZoomIn;
    QRect m_rectZoomOut;
    QMap<int, QRect> m_floorBtnRects;
};

#endif // CAMPUSMAPVIEW_H
