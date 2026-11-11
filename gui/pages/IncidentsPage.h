#ifndef INCIDENTSPAGE_H
#define INCIDENTSPAGE_H

#include "../widgets/BasePage.h"
#include "../AppContext.h"

class DataTable;
class QPushButton;
class QComboBox;

class IncidentsPage : public BasePage {
    Q_OBJECT
public:
    explicit IncidentsPage(const AppContext& ctx, QWidget* parent = nullptr);

    QString title() const override { return "Incidents Queue"; }
    void refresh() override;

private slots:
    void onAddIncident();
    void onAssignAmbulance();
    void onResolveIncident();
    void onFilterChanged();

private:
    AppContext   m_ctx;
    DataTable*   m_table      = nullptr;
    QPushButton* m_addBtn     = nullptr;
    QPushButton* m_assignBtn  = nullptr;
    QPushButton* m_resolveBtn = nullptr;
    QComboBox*   m_filterBox  = nullptr;
};

#endif // INCIDENTSPAGE_H
