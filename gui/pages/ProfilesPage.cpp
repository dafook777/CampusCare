#include "ProfilesPage.h"
#include "../widgets/DataTable.h"
#include "../widgets/Badge.h"
#include "../widgets/StudentProfileCardDialog.h"
#include "../services/StudentService.h"
#include "../services/FacultyService.h"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTableWidget>
#include <QVBoxLayout>

ProfilesPage::ProfilesPage(const AppContext& ctx, const QString& mode, QWidget* parent)
    : BasePage(parent), m_ctx(ctx), m_mode(mode)
{
    // Infer default mode if empty
    if (m_mode.isEmpty()) {
        if (m_ctx.role == "student" || m_ctx.role == "faculty") {
            m_mode = "my_profile";
        } else {
            m_mode = "user_profiles";
        }
    }

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    if (m_mode == "my_profile") {
        // ── Single Profile View (Open by default for Student / Faculty) ──
        auto* scroll = new QScrollArea(this);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        scroll->setStyleSheet("background: transparent;");

        auto* container = new QWidget(scroll);
        container->setStyleSheet("background: transparent;");
        auto* cLayout = new QVBoxLayout(container);
        cLayout->setContentsMargins(0, 0, 0, 0);
        cLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

        UserProfileData u;
        if (m_ctx.role == "faculty") {
            u = UserProfileData::fromFaculty(FacultyService::getById(m_ctx.userId));
        } else {
            u = UserProfileData::fromStudent(StudentService::getById(m_ctx.userId));
        }

        m_profileCard = new StudentProfileCardWidget(u, m_ctx, container);
        connect(m_profileCard, &StudentProfileCardWidget::profileUpdated, this, &ProfilesPage::refresh);
        cLayout->addWidget(m_profileCard);

        scroll->setWidget(container);
        layout->addWidget(scroll, 1);
    } else {
        // ── Directory View (Student Profiles for Faculty, User Profiles for Admin) ──
        auto* topRow = new QHBoxLayout;
        topRow->setSpacing(12);

        QString tipText = (m_mode == "user_profiles")
            ? QString::fromUtf8("\xF0\x9F\x92\xA1  Tip: Click any user row or [ View Profile ] to inspect student or faculty profile cards.")
            : QString::fromUtf8("\xF0\x9F\x92\xA1  Tip: Click any student row or [ View Profile ] to inspect their profile card.");

        auto* tip = new QLabel(tipText, this);
        tip->setStyleSheet("color: #38bdf8; font-size: 13px; font-weight: 600; padding: 2px 0;");
        topRow->addWidget(tip);
        topRow->addStretch(1);

        // Role filter tabs for Admin
        if (m_mode == "user_profiles") {
            auto makeFilterBtn = [this](const QString& text, const QString& filterId) {
                auto* btn = new QPushButton(text, this);
                btn->setCursor(Qt::PointingHandCursor);
                btn->setFixedHeight(30);
                connect(btn, &QPushButton::clicked, this, [this, filterId]() {
                    setRoleFilter(filterId);
                });
                return btn;
            };

            m_filterAllBtn = makeFilterBtn("All Users", "all");
            m_filterStudentBtn = makeFilterBtn(QString::fromUtf8("\xF0\x9F\x8E\x93  Students"), "student");
            m_filterFacultyBtn = makeFilterBtn(QString::fromUtf8("\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x8F\xAB  Faculty"), "faculty");

            topRow->addWidget(m_filterAllBtn);
            topRow->addWidget(m_filterStudentBtn);
            topRow->addWidget(m_filterFacultyBtn);
        }

        layout->addLayout(topRow);

        m_table = new DataTable(this);
        layout->addWidget(m_table, 1);

        // Connect cell click anywhere in row to open profile card
        connect(m_table->table(), &QTableWidget::cellClicked, this, [this](int row, int /*col*/) {
            if (row >= 0 && row < m_cachedUsers.size()) {
                openProfileCard(m_cachedUsers[row]);
            }
        });
    }

    refresh();
}

QString ProfilesPage::title() const
{
    if (m_mode == "my_profile") return "My Profile";
    if (m_mode == "student_profiles") return "Student Profiles";
    if (m_mode == "user_profiles") return "User Profiles";
    return (m_ctx.role == "student" || m_ctx.role == "faculty") ? "My Profile" : "User Profiles";
}

void ProfilesPage::setRoleFilter(const QString& filter)
{
    m_roleFilter = filter;
    refresh();
}

void ProfilesPage::refresh()
{
    if (m_mode == "my_profile") {
        if (m_profileCard) {
            UserProfileData u;
            if (m_ctx.role == "faculty") {
                u = UserProfileData::fromFaculty(FacultyService::getById(m_ctx.userId));
            } else {
                u = UserProfileData::fromStudent(StudentService::getById(m_ctx.userId));
            }
            m_profileCard->setUser(u);
        }
        return;
    }

    if (!m_table) return;

    // Update filter button styles if Admin
    if (m_mode == "user_profiles" && m_filterAllBtn) {
        auto updateBtnStyle = [](QPushButton* btn, bool active) {
            if (active) {
                btn->setStyleSheet(
                    "background: #0288d1; color: white; border: none; border-radius: 6px;"
                    "font-size: 11px; font-weight: 700; padding: 4px 12px;"
                );
            } else {
                btn->setStyleSheet(
                    "background: rgba(30, 41, 59, 180); color: #94a3b8; border: 1px solid rgba(255, 255, 255, 25);"
                    "border-radius: 6px; font-size: 11px; font-weight: 600; padding: 4px 12px;"
                );
            }
        };

        updateBtnStyle(m_filterAllBtn, m_roleFilter == "all");
        updateBtnStyle(m_filterStudentBtn, m_roleFilter == "student");
        updateBtnStyle(m_filterFacultyBtn, m_roleFilter == "faculty");
    }

    // Load users
    QVector<UserProfileData> allList;
    const auto students = StudentService::loadAll();
    for (const auto& s : students) {
        allList.append(UserProfileData::fromStudent(s));
    }

    if (m_mode == "user_profiles") {
        const auto faculties = FacultyService::loadAll();
        for (const auto& f : faculties) {
            allList.append(UserProfileData::fromFaculty(f));
        }
    }

    // Filter by role
    m_cachedUsers.clear();
    for (const auto& u : allList) {
        if (m_roleFilter == "student" && u.role != "student") continue;
        if (m_roleFilter == "faculty" && u.role != "faculty") continue;
        m_cachedUsers.append(u);
    }

    // Build Table
    const bool isUserProfiles = (m_mode == "user_profiles");
    QStringList headers;
    if (isUserProfiles) {
        headers = {"ID", "Name", "Role", "Department", "Phone", "Blood Group", "Gender", "Status", "Profile"};
    } else {
        headers = {"ID", "Name", "Department", "Phone", "Blood Group", "Gender", "Status", "Profile"};
    }

    // Write clean, normal text in every single cell — exactly like Name, Phone, and Department
    QVector<QStringList> rows;
    for (const auto& u : m_cachedUsers) {
        const QString roleStr = (u.role == "faculty") ? "Faculty" : "Student";
        const QString statusStr = u.isActive ? "Active" : "Inactive";
        if (isUserProfiles) {
            rows.append({
                u.id,
                u.name,
                roleStr,
                u.department,
                u.phone,
                u.bloodGroup,
                u.gender,
                statusStr,
                "View Profile"
            });
        } else {
            rows.append({
                u.id,
                u.name,
                u.department,
                u.phone,
                u.bloodGroup,
                u.gender,
                statusStr,
                "View Profile"
            });
        }
    }

    m_table->setData(headers, rows);

    // Style text items directly: high-contrast, crystal-clear readability, zero widget clipping
    auto* tbl = m_table->table();
    const int roleCol = isUserProfiles ? 2 : -1;
    const int statusCol = isUserProfiles ? 7 : 6;
    const int profileCol = isUserProfiles ? 8 : 7;

    QFont boldFont;
    boldFont.setBold(true);

    for (int i = 0; i < m_cachedUsers.size(); ++i) {
        const auto& u = m_cachedUsers[i];

        // 1. Role text
        if (roleCol != -1) {
            if (auto* item = tbl->item(i, roleCol)) {
                item->setForeground(QBrush(u.role == "faculty" ? QColor("#c084fc") : QColor("#38bdf8")));
                item->setFont(boldFont);
                item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            }
        }

        // 2. Status text
        if (auto* item = tbl->item(i, statusCol)) {
            item->setForeground(QBrush(u.isActive ? QColor("#34d399") : QColor("#f87171")));
            item->setFont(boldFont);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        }

        // 3. Profile / Action text
        if (auto* item = tbl->item(i, profileCol)) {
            item->setForeground(QBrush(QColor("#00e5ff")));
            item->setFont(boldFont);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        }
    }

    // Configure generous column widths so IDs, names, and departments never get truncated with ellipsis!
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Name stretches
    if (isUserProfiles) {
        tbl->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch); // Department stretches
        tbl->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
        tbl->setColumnWidth(0, 140); // ID: full 11-digit roll numbers fit completely
        tbl->setColumnWidth(2, 90);  // Role
        tbl->setColumnWidth(4, 120); // Phone
        tbl->setColumnWidth(5, 100); // Blood group
        tbl->setColumnWidth(6, 85);  // Gender
        tbl->setColumnWidth(7, 85);  // Status
        tbl->setColumnWidth(8, 110); // Profile
    } else {
        tbl->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); // Department stretches
        tbl->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
        tbl->setColumnWidth(0, 140); // ID
        tbl->setColumnWidth(3, 120); // Phone
        tbl->setColumnWidth(4, 100); // Blood group
        tbl->setColumnWidth(5, 85);  // Gender
        tbl->setColumnWidth(6, 85);  // Status
        tbl->setColumnWidth(7, 110); // Profile
    }
}

void ProfilesPage::openProfileCard(const UserProfileData& user)
{
    StudentProfileCardDialog dlg(user, m_ctx, this);
    connect(&dlg, &StudentProfileCardDialog::profileUpdated, this, &ProfilesPage::refresh);
    dlg.exec();
}
