#ifndef MAPPAGE_H
#define MAPPAGE_H

#include "../widgets/BasePage.h"

class QComboBox;
class QPushButton;
class QLabel;
class CampusMapView;

class MapPage : public BasePage {
    Q_OBJECT
public:
    explicit MapPage(QWidget* parent = nullptr);

    QString title() const override { return "Campus Map and Route"; }
    void refresh() override;

private slots:
    void onFindRoute();
    void onFindEmergency();

private:
    QComboBox*     m_fromCombo   = nullptr;
    QComboBox*     m_toCombo     = nullptr;
    QComboBox*     m_algoCombo   = nullptr;
    QPushButton*   m_findBtn     = nullptr;
    QPushButton*   m_emergBtn    = nullptr;
    QLabel*        m_resultPath  = nullptr;
    QLabel*        m_resultStats = nullptr;
    CampusMapView* m_mapView     = nullptr;
};

#endif // MAPPAGE_H

