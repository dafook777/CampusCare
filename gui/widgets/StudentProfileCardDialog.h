#ifndef STUDENTPROFILECARDDIALOG_H
#define STUDENTPROFILECARDDIALOG_H

#include <QDialog>
#include <QWidget>
#include "../AppContext.h"
#include "../services/StudentService.h"
#include "../services/FacultyService.h"

class QLabel;
class QPushButton;

struct UserProfileData {
    QString id;
    QString role = "student"; // "student", "faculty", "admin"
    QString name;
    QString department;
    QString phone;
    QString bloodGroup;
    QString gender;
    bool isActive = true;
    QString designation;

    static UserProfileData fromStudent(const StudentItem& s) {
        UserProfileData p;
        p.id = s.studentId;
        p.role = "student";
        p.name = s.name;
        p.department = s.department;
        p.phone = s.phone;
        p.bloodGroup = s.bloodGroup;
        p.gender = s.gender;
        p.isActive = s.isActive;
        p.designation = "Student";
        return p;
    }

    static UserProfileData fromFaculty(const FacultyItem& f) {
        UserProfileData p;
        p.id = f.facultyId;
        p.role = "faculty";
        p.name = f.name;
        p.department = f.department;
        p.phone = f.phone;
        p.bloodGroup = f.bloodGroup;
        p.gender = f.gender;
        p.isActive = f.isActive;
        p.designation = f.designation.isEmpty() ? "Faculty Member" : f.designation;
        return p;
    }
};

class StudentProfileCardWidget : public QWidget {
    Q_OBJECT
public:
    explicit StudentProfileCardWidget(const UserProfileData& user, const AppContext& ctx, QWidget* parent = nullptr);
    explicit StudentProfileCardWidget(const StudentItem& student, const AppContext& ctx, QWidget* parent = nullptr);
    explicit StudentProfileCardWidget(const FacultyItem& faculty, const AppContext& ctx, QWidget* parent = nullptr);

    void setUser(const UserProfileData& u);
    void setStudent(const StudentItem& s);
    void setFaculty(const FacultyItem& f);

    const UserProfileData& userData() const { return m_user; }

signals:
    void profileUpdated(const UserProfileData& user);

public slots:
    void onChangePhoto();
    void onEditInfo();
    void rebuildCard();

private:
    QPixmap renderAvatar(const QString& id, const QString& name, const QString& role, int size);

    UserProfileData m_user;
    AppContext      m_ctx;

    QLabel*      m_bannerSub     = nullptr;
    QLabel*      m_photoLabel    = nullptr;
    QLabel*      m_nameLabel     = nullptr;
    QLabel*      m_idTagLabel    = nullptr;
    QLabel*      m_deptLabel     = nullptr;
    QLabel*      m_bloodBadge    = nullptr;
    QLabel*      m_phoneVal      = nullptr;
    QLabel*      m_genderVal     = nullptr;
    QLabel*      m_statusVal     = nullptr;
    QLabel*      m_clearanceTitle = nullptr;
    QLabel*      m_clearanceSub   = nullptr;
};

class StudentProfileCardDialog : public QDialog {
    Q_OBJECT
public:
    explicit StudentProfileCardDialog(const UserProfileData& user, const AppContext& ctx, QWidget* parent = nullptr);
    explicit StudentProfileCardDialog(const StudentItem& student, const AppContext& ctx, QWidget* parent = nullptr);
    explicit StudentProfileCardDialog(const FacultyItem& faculty, const AppContext& ctx, QWidget* parent = nullptr);

signals:
    void profileUpdated(const UserProfileData& user);

private:
    StudentProfileCardWidget* m_card = nullptr;
};

#endif // STUDENTPROFILECARDDIALOG_H
