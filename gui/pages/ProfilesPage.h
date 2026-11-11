#ifndef PROFILESPAGE_H
#define PROFILESPAGE_H
#include "../widgets/BasePage.h"
#include "../AppContext.h"
class DataTable;
class QLabel;
class ProfilesPage : public BasePage {
    Q_OBJECT
public:
    explicit ProfilesPage(const AppContext& ctx, QWidget* parent = nullptr);
    QString title() const override {
        return m_ctx.role == "student" ? "My Profile" : "Student Profiles";
    }
    void refresh() override;
private slots:
    void onEdit();
private:
    AppContext m_ctx;
    DataTable* m_table = nullptr;
    QWidget* m_profileCard = nullptr;
};
#endif
