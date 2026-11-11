#ifndef AMBULANCEPAGE_H
#define AMBULANCEPAGE_H

#include "../widgets/BasePage.h"
#include "../AppContext.h"
#include "../services/AmbulanceService.h"

class DataTable;
class QPushButton;
class QLabel;

class AmbulancePage : public BasePage {
    Q_OBJECT
public:
    explicit AmbulancePage(const AppContext& ctx, QWidget* parent = nullptr);

    QString title() const override { return "Ambulance Fleet & Dispatch"; }
    void refresh() override;

private slots:
    void onReleaseAmbulance();

private:
    AppContext   m_ctx;
    DataTable*   m_table     = nullptr;
    QPushButton* m_releaseBtn = nullptr;
    QLabel*      m_dispatchSummary = nullptr;
};

#endif // AMBULANCEPAGE_H

