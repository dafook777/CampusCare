#ifndef CONTACTSPAGE_H
#define CONTACTSPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class DataTable;
class ContactsPage : public BasePage {
    Q_OBJECT
public:
    explicit ContactsPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override { return "Emergency Contacts"; }
    void refresh() override;
private slots:
    void onAdd();
    void onDelete();
private:
    AppContext m_ctx;
    DataTable* m_table = nullptr;
};
#endif

