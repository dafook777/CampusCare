#ifndef MESSAGESPAGE_H
#define MESSAGESPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class DataTable;
class MessagesPage : public BasePage {
    Q_OBJECT
public:
    explicit MessagesPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override { return "Messages"; }
    void refresh() override;
private slots:
    void onSend();
private:
    AppContext m_ctx;
    DataTable* m_table = nullptr;
};
#endif

