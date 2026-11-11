#ifndef CONCERNSPAGE_H
#define CONCERNSPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class DataTable;
class QComboBox;
class ConcernsPage : public BasePage {
    Q_OBJECT
public:
    explicit ConcernsPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override { return "Concerns"; }
    void refresh() override;
private slots:
    void onSubmit();
    void onResolve();
    void onFilterChanged();
private:
    AppContext m_ctx;
    DataTable* m_table = nullptr;
    QComboBox* m_filterBox = nullptr;
};
#endif

