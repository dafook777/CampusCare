#ifndef PROFILESPAGE_H
#define PROFILESPAGE_H

#include "../widgets/BasePage.h"
#include "../AppContext.h"
#include "../widgets/StudentProfileCardDialog.h"

class DataTable;
class QPushButton;

class ProfilesPage : public BasePage {
    Q_OBJECT
public:
    explicit ProfilesPage(const AppContext& ctx, const QString& mode = QString(), QWidget* parent = nullptr);
    QString title() const override;
    void refresh() override;

private slots:
    void openProfileCard(const UserProfileData& user);
    void setRoleFilter(const QString& filter);

private:
    AppContext                m_ctx;
    QString                   m_mode;
    DataTable*                m_table       = nullptr;
    StudentProfileCardWidget* m_profileCard = nullptr;
    QString                   m_roleFilter  = "all";
    QVector<UserProfileData>  m_cachedUsers;

    QPushButton* m_filterAllBtn     = nullptr;
    QPushButton* m_filterStudentBtn = nullptr;
    QPushButton* m_filterFacultyBtn = nullptr;
};

#endif // PROFILESPAGE_H
