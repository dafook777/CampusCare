#ifndef DISPATCHLOGPAGE_H
#define DISPATCHLOGPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class DataTable;
class DispatchLogPage : public BasePage {
    Q_OBJECT
public:
    explicit DispatchLogPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override { return "Dispatch Log"; }
    void refresh() override;
private:
    AppContext m_ctx;
    DataTable* m_table = nullptr;
};
#endif

