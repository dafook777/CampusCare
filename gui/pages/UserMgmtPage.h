#ifndef USERMGMTPAGE_H
#define USERMGMTPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class DataTable;
class UserMgmtPage : public BasePage {
    Q_OBJECT
public:
    explicit UserMgmtPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override { return "User Management"; }
    void refresh() override;
private slots:
    void onAddUser();
    void onDeleteUser();
    void onChangeRole();
private:
    AppContext m_ctx;
    DataTable* m_table = nullptr;
};
#endif
