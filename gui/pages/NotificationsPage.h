#ifndef NOTIFICATIONSPAGE_H
#define NOTIFICATIONSPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class DataTable;
class QLabel;
class NotificationsPage : public BasePage {
    Q_OBJECT
public:
    explicit NotificationsPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override { return "Notifications"; }
    void refresh() override;
private slots:
    void onMarkRead();
private:
    AppContext m_ctx;
    DataTable* m_table = nullptr;
    QLabel* m_unreadLbl = nullptr;
};
#endif

