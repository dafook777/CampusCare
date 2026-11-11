#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class QLabel;
class DashboardPage : public BasePage {
    Q_OBJECT
public:
    explicit DashboardPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override { return "Dashboard"; }
    void refresh() override;
private:
    AppContext m_ctx;
    QLabel* m_incTotal = nullptr;
    QLabel* m_incOpen = nullptr;
    QLabel* m_incResolved = nullptr;
    QLabel* m_ambAvail = nullptr;
    QLabel* m_ambBusy = nullptr;
    QLabel* m_userCount = nullptr;
    QLabel* m_studentCount = nullptr;
    QLabel* m_notifUnread = nullptr;
    QLabel* m_concernCount = nullptr;
};
#endif

