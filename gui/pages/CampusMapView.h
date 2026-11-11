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
    explicit CampusMapView(QWidget* parent = nullptr);

    void setHighlightedPath(const QStringList& path);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QMap<QString, QPointF> m_coords;
    QStringList m_highlightedPath;
};

#endif // CAMPUSMAPVIEW_H

