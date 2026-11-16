#include "StudentProfileCardDialog.h"
#include "../services/DataPath.h"
#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QRegularExpression>
#include <QVBoxLayout>

namespace {

static QPixmap getGehuLogo(int size = 32)
{
    QPixmap raw;
    if (QFile::exists(":/gehu_logo.png")) {
        raw.load(":/gehu_logo.png");
    } else if (QFile::exists(":/gehu_logo.jpg")) {
        raw.load(":/gehu_logo.jpg");
    }
    if (raw.isNull()) {
        raw.load(DataPath::resolve("resources/gehu_logo.png"));
    }
    if (raw.isNull()) {
        raw.load(DataPath::resolve("gehu_logo.png"));
    }

    if (raw.isNull()) {
        QPixmap fallback(size, size);
        fallback.fill(Qt::transparent);
        QPainter p(&fallback);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor("#0288d1"));
        p.drawEllipse(1, 1, size - 2, size - 2);
        return fallback;
    }

    QPixmap scaled = raw.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap result(size, size);
    result.fill(Qt::transparent);
    QPainter p(&result);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPainterPath clip;
    clip.addEllipse(0.5, 0.5, size - 1.0, size - 1.0);
    p.setClipPath(clip);
    p.drawPixmap(0, 0, scaled);
    p.end();
    return result;
}

} // namespace

// ─────────────────────────────────────────────────────────────
// StudentProfileCardWidget Constructors
// ─────────────────────────────────────────────────────────────
StudentProfileCardWidget::StudentProfileCardWidget(const UserProfileData& user, const AppContext& ctx, QWidget* parent)
    : QWidget(parent), m_user(user), m_ctx(ctx)
{
    setFixedWidth(520);
    setObjectName("profileCardContainer");

    const bool isFaculty = (m_user.role == "faculty");
    const QString accentBorder = isFaculty ? "#a855f7" : "#00e5ff";

    setStyleSheet(QString(
        "QWidget#profileCardContainer {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0f172a, stop:0.5 #090d16, stop:1 #030712);"
        "  border: 2px solid %1;"
        "  border-radius: 16px;"
        "}"
        "QFrame#statCard {"
        "  background: rgba(30, 41, 59, 180);"
        "  border: 1px solid rgba(255, 255, 255, 25);"
        "  border-radius: 10px;"
        "}"
        "QFrame#statCard:hover {"
        "  border: 1px solid #38bdf8;"
        "  background: rgba(30, 41, 59, 230);"
        "}"
        "QLabel#statKey {"
        "  color: #94a3b8; font-size: 11px; font-weight: 600; text-transform: uppercase; letter-spacing: 0.5px;"
        "}"
        "QLabel#statVal {"
        "  color: #f8fafc; font-size: 14px; font-weight: 700;"
        "}"
        "QPushButton#actionBtn {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0288d1, stop:1 #00e5ff);"
        "  color: #030712; border: none; border-radius: 8px; font-size: 13px; font-weight: 700;"
        "  padding: 8px 16px;"
        "}"
        "QPushButton#actionBtn:hover { background: #38bdf8; }"
        "QPushButton#secondaryBtn {"
        "  background: rgba(30, 41, 59, 220); color: #e2e8f0; border: 1px solid rgba(255, 255, 255, 40);"
        "  border-radius: 8px; font-size: 12px; font-weight: 600; padding: 6px 12px;"
        "}"
        "QPushButton#secondaryBtn:hover { border-color: #00e5ff; color: #00e5ff; }"
    ).arg(accentBorder));

    auto* mainVBox = new QVBoxLayout(this);
    mainVBox->setContentsMargins(24, 20, 24, 20);
    mainVBox->setSpacing(14);

    // ── 1. Top Card Header ──
    auto* topBar = new QHBoxLayout;
    topBar->setSpacing(10);

    auto* uniLogo = new QLabel(this);
    uniLogo->setPixmap(getGehuLogo(32));
    uniLogo->setFixedSize(32, 32);
    topBar->addWidget(uniLogo);

    auto* titleVBox = new QVBoxLayout;
    titleVBox->setSpacing(1);
    auto* bannerTitle = new QLabel("GRAPHIC ERA HILL UNIVERSITY", this);
    bannerTitle->setStyleSheet("color: #f8fafc; font-size: 13px; font-weight: 800; letter-spacing: 1px;");

    m_bannerSub = new QLabel(isFaculty ? "FACULTY & STAFF PROFILE" : "STUDENT PROFILE", this);
    m_bannerSub->setStyleSheet(QString("color: %1; font-size: 10px; font-weight: 700; letter-spacing: 0.8px;")
                                   .arg(isFaculty ? "#c084fc" : "#00e5ff"));
    titleVBox->addWidget(bannerTitle);
    titleVBox->addWidget(m_bannerSub);
    topBar->addLayout(titleVBox);

    topBar->addStretch(1);

    auto* liveBadge = new QLabel(QString::fromUtf8("\xE2\x97\x8F  VERIFIED ACTIVE"), this);
    liveBadge->setStyleSheet(
        "color: #10b981; background: rgba(16, 185, 129, 30); border: 1px solid rgba(16, 185, 129, 80);"
        "border-radius: 12px; padding: 4px 10px; font-size: 10px; font-weight: 700;"
    );
    topBar->addWidget(liveBadge);

    mainVBox->addLayout(topBar);

    // Separator line
    auto* sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setStyleSheet("color: rgba(255, 255, 255, 30); max-height: 1px;");
    mainVBox->addWidget(sep1);

    // ── 2. Hero Section (Photo + User Tag) ──
    auto* heroHBox = new QHBoxLayout;
    heroHBox->setSpacing(20);

    auto* avatarVBox = new QVBoxLayout;
    avatarVBox->setSpacing(6);
    avatarVBox->setAlignment(Qt::AlignCenter);

    m_photoLabel = new QLabel(this);
    m_photoLabel->setFixedSize(112, 112);
    m_photoLabel->setAlignment(Qt::AlignCenter);
    m_photoLabel->setStyleSheet(QString(
        "border: 2px solid %1; border-radius: 18px; background: rgba(15, 23, 42, 220);"
    ).arg(accentBorder));
    avatarVBox->addWidget(m_photoLabel);

    auto* uploadPhotoBtn = new QPushButton(QString::fromUtf8("\xF0\x9F\x93\xB7  Upload Photo"), this);
    uploadPhotoBtn->setObjectName("secondaryBtn");
    uploadPhotoBtn->setCursor(Qt::PointingHandCursor);
    uploadPhotoBtn->setFixedHeight(26);
    connect(uploadPhotoBtn, &QPushButton::clicked, this, &StudentProfileCardWidget::onChangePhoto);
    avatarVBox->addWidget(uploadPhotoBtn);

    heroHBox->addLayout(avatarVBox);

    auto* idDetailsVBox = new QVBoxLayout;
    idDetailsVBox->setSpacing(4);
    idDetailsVBox->setAlignment(Qt::AlignVCenter);

    m_nameLabel = new QLabel(this);
    m_nameLabel->setStyleSheet("color: #f8fafc; font-size: 21px; font-weight: 800; letter-spacing: 0.5px;");
    idDetailsVBox->addWidget(m_nameLabel);

    m_idTagLabel = new QLabel(this);
    m_idTagLabel->setStyleSheet(QString(
        "color: %1; font-family: 'Consolas', 'Segoe UI', monospace; font-size: 13px; font-weight: 700;"
        "background: rgba(0, 229, 255, 18); border: 1px solid rgba(0, 229, 255, 55);"
        "border-radius: 6px; padding: 2px 8px; max-width: 200px;"
    ).arg(isFaculty ? "#c084fc" : "#00e5ff"));
    idDetailsVBox->addWidget(m_idTagLabel);

    m_deptLabel = new QLabel(this);
    m_deptLabel->setStyleSheet("color: #94a3b8; font-size: 12px; font-weight: 600; padding-top: 4px;");
    idDetailsVBox->addWidget(m_deptLabel);

    auto* uniCamp = new QLabel("Campus: GEHU Dehradun Central", this);
    uniCamp->setStyleSheet("color: #64748b; font-size: 11px; font-weight: 500;");
    idDetailsVBox->addWidget(uniCamp);

    heroHBox->addLayout(idDetailsVBox, 1);
    mainVBox->addLayout(heroHBox);

    // Separator line
    auto* sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setStyleSheet("color: rgba(255, 255, 255, 30); max-height: 1px;");
    mainVBox->addWidget(sep2);

    // ── 3. Vitals & Parameters Grid ──
    auto* gridSectionTitle = new QLabel(QString::fromUtf8("\xE2\x9A\xA1  PROFILE VITALS & PARAMETERS"), this);
    gridSectionTitle->setStyleSheet("color: #e2e8f0; font-size: 12px; font-weight: 700; letter-spacing: 1px;");
    mainVBox->addWidget(gridSectionTitle);

    auto* grid = new QGridLayout;
    grid->setSpacing(10);

    auto makeStatCard = [this](const QString& key, QLabel*& valLabel, const QString& initialVal, const QString& accentColor = "#f8fafc") {
        auto* card = new QFrame(this);
        card->setObjectName("statCard");
        auto* v = new QVBoxLayout(card);
        v->setContentsMargins(12, 10, 12, 10);
        v->setSpacing(4);

        auto* k = new QLabel(key, card);
        k->setObjectName("statKey");
        v->addWidget(k);

        valLabel = new QLabel(initialVal, card);
        valLabel->setObjectName("statVal");
        valLabel->setStyleSheet("color: " + accentColor + "; font-size: 14px; font-weight: 700;");
        v->addWidget(valLabel);
        return card;
    };

    grid->addWidget(makeStatCard(QString::fromUtf8("\xF0\x9F\xAA\xB8  Blood Group"), m_bloodBadge, m_user.bloodGroup, "#ef4444"), 0, 0);
    grid->addWidget(makeStatCard(QString::fromUtf8("\xF0\x9F\x93\x9E  Emergency Comms"), m_phoneVal, m_user.phone, "#38bdf8"), 0, 1);
    grid->addWidget(makeStatCard(QString::fromUtf8("\xF0\x9F\x91\xA4  Gender / Class"), m_genderVal, m_user.gender, "#cbd5e1"), 1, 0);
    grid->addWidget(makeStatCard(QString::fromUtf8("\xF0\x9F\x9B\xA1  Safety Status"), m_statusVal, m_user.isActive ? "CLEAR / ACTIVE" : "FLAGGED", "#10b981"), 1, 1);

    mainVBox->addLayout(grid);

    // ── 4. Campus Clearance Card ──
    auto* clearanceCard = new QFrame(this);
    clearanceCard->setObjectName("statCard");
    auto* clH = new QHBoxLayout(clearanceCard);
    clH->setContentsMargins(14, 10, 14, 10);
    clH->setSpacing(12);

    auto* clIcon = new QLabel(QString::fromUtf8(isFaculty ? "\xF0\x9F\x8F\x9B\xEF\xB8\x8F" : "\xF0\x9F\x8F\xAB"), clearanceCard);
    clIcon->setStyleSheet("font-size: 24px;");
    clH->addWidget(clIcon);

    auto* clV = new QVBoxLayout;
    clV->setSpacing(2);
    m_clearanceTitle = new QLabel(isFaculty ? "Faculty Clearance Level 2 - Authorized"
                                            : "Campus Clearance Level 1 - Validated", clearanceCard);
    m_clearanceTitle->setStyleSheet("color: #f8fafc; font-size: 13px; font-weight: 700;");

    m_clearanceSub = new QLabel(isFaculty ? "Authorized for academic oversight, student concerns, and institutional facilities."
                                          : "All biometric credentials and facility authorizations in good standing.", clearanceCard);
    m_clearanceSub->setStyleSheet("color: #94a3b8; font-size: 11px;");
    clV->addWidget(m_clearanceTitle);
    clV->addWidget(m_clearanceSub);
    clH->addLayout(clV, 1);

    mainVBox->addWidget(clearanceCard);

    // ── 5. Action Buttons ──
    auto* btnHBox = new QHBoxLayout;
    btnHBox->setSpacing(10);

    auto* editBtn = new QPushButton(QString::fromUtf8("\xE2\x9C\x8F  Edit Info"), this);
    editBtn->setObjectName("secondaryBtn");
    editBtn->setCursor(Qt::PointingHandCursor);
    editBtn->setFixedHeight(36);
    connect(editBtn, &QPushButton::clicked, this, &StudentProfileCardWidget::onEditInfo);
    btnHBox->addWidget(editBtn);

    btnHBox->addStretch(1);
    mainVBox->addLayout(btnHBox);

    rebuildCard();
}

StudentProfileCardWidget::StudentProfileCardWidget(const StudentItem& student, const AppContext& ctx, QWidget* parent)
    : StudentProfileCardWidget(UserProfileData::fromStudent(student), ctx, parent)
{
}

StudentProfileCardWidget::StudentProfileCardWidget(const FacultyItem& faculty, const AppContext& ctx, QWidget* parent)
    : StudentProfileCardWidget(UserProfileData::fromFaculty(faculty), ctx, parent)
{
}

void StudentProfileCardWidget::setUser(const UserProfileData& u)
{
    m_user = u;
    rebuildCard();
}

void StudentProfileCardWidget::setStudent(const StudentItem& s)
{
    m_user = UserProfileData::fromStudent(s);
    rebuildCard();
}

void StudentProfileCardWidget::setFaculty(const FacultyItem& f)
{
    m_user = UserProfileData::fromFaculty(f);
    rebuildCard();
}

QPixmap StudentProfileCardWidget::renderAvatar(const QString& id, const QString& name, const QString& role, int size)
{
    // Check if custom uploaded photo exists
    QString photoFile = (role == "faculty") ? FacultyService::photoPath(id) : StudentService::photoPath(id);
    if (photoFile.isEmpty()) photoFile = StudentService::photoPath(id);
    if (photoFile.isEmpty()) photoFile = FacultyService::photoPath(id);

    if (!photoFile.isEmpty()) {
        QPixmap src(photoFile);
        if (!src.isNull()) {
            QPixmap out(size, size);
            out.fill(Qt::transparent);
            QPainter p(&out);
            p.setRenderHint(QPainter::Antialiasing);
            p.setRenderHint(QPainter::SmoothPixmapTransform);

            QPainterPath clip;
            clip.addRoundedRect(0, 0, size, size, 16, 16);
            p.setClipPath(clip);

            QPixmap scaled = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            int sx = (scaled.width() - size) / 2;
            int sy = (scaled.height() - size) / 2;
            p.drawPixmap(0, 0, scaled.copy(sx, sy, size, size));
            return out;
        }
    }

    // High-Tech Avatar Procedural Fallback
    QPixmap out(size, size);
    out.fill(Qt::transparent);
    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing);

    const bool isFaculty = (role == "faculty");
    QLinearGradient bgGrad(0, 0, size, size);
    if (isFaculty) {
        bgGrad.setColorAt(0, QColor("#3b0764"));
        bgGrad.setColorAt(0.5, QColor("#1e1b4b"));
        bgGrad.setColorAt(1, QColor("#0f172a"));
    } else {
        bgGrad.setColorAt(0, QColor("#1e293b"));
        bgGrad.setColorAt(0.5, QColor("#0f172a"));
        bgGrad.setColorAt(1, QColor("#0284c7"));
    }
    p.setBrush(bgGrad);
    p.setPen(QPen(QColor(isFaculty ? "#c084fc" : "#00e5ff"), 2.0));
    p.drawRoundedRect(1, 1, size - 2, size - 2, 16, 16);

    p.setPen(QPen(QColor(isFaculty ? 192 : 0, isFaculty ? 132 : 229, 255, 60), 1.5, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(6, 6, size - 12, size - 12, 12, 12);

    QString initials = isFaculty ? "FC" : "ST";
    const QString trimmed = name.trimmed();
    if (!trimmed.isEmpty()) {
        const auto parts = trimmed.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() >= 2) {
            initials = QString(parts[0][0].toUpper()) + QString(parts[1][0].toUpper());
        } else if (trimmed.length() >= 2) {
            initials = trimmed.left(2).toUpper();
        } else {
            initials = trimmed.left(1).toUpper();
        }
    }

    p.setFont(QFont("Segoe UI", size / 3, QFont::Bold));
    p.setPen(QColor("#f8fafc"));
    p.drawText(QRect(0, 0, size, size - 8), Qt::AlignCenter, initials);

    p.setFont(QFont("Segoe UI", 9, QFont::Bold));
    p.setPen(QColor(isFaculty ? "#c084fc" : "#38bdf8"));
    p.drawText(QRect(0, size - 24, size, 18), Qt::AlignCenter, isFaculty ? "FACULTY" : "STUDENT");

    return out;
}

void StudentProfileCardWidget::rebuildCard()
{
    const bool isFaculty = (m_user.role == "faculty");
    m_photoLabel->setPixmap(renderAvatar(m_user.id, m_user.name, m_user.role, 108));

    if (m_bannerSub) {
        m_bannerSub->setText(isFaculty ? "FACULTY & STAFF PROFILE" : "STUDENT PROFILE");
        m_bannerSub->setStyleSheet(QString("color: %1; font-size: 10px; font-weight: 700; letter-spacing: 0.8px;")
                                       .arg(isFaculty ? "#c084fc" : "#00e5ff"));
    }

    m_nameLabel->setText(m_user.name.isEmpty() ? (isFaculty ? "Faculty Member" : "Unknown Student") : m_user.name);
    m_idTagLabel->setText(QString("%1: %2").arg(isFaculty ? "FACULTY ID" : "REG ID", m_user.id.isEmpty() ? "N/A" : m_user.id));
    m_idTagLabel->setStyleSheet(QString(
        "color: %1; font-family: 'Consolas', 'Segoe UI', monospace; font-size: 13px; font-weight: 700;"
        "background: rgba(0, 229, 255, 18); border: 1px solid rgba(0, 229, 255, 55);"
        "border-radius: 6px; padding: 2px 8px; max-width: 200px;"
    ).arg(isFaculty ? "#c084fc" : "#00e5ff"));

    m_deptLabel->setText(QString("Dept: %1").arg(m_user.department.isEmpty() ? "Academic Department" : m_user.department));

    m_bloodBadge->setText(m_user.bloodGroup.isEmpty() ? "Unknown" : m_user.bloodGroup);
    m_phoneVal->setText(m_user.phone.isEmpty() ? "Not registered" : m_user.phone);
    m_genderVal->setText(m_user.gender.isEmpty() ? "Unspecified" : m_user.gender);
    m_statusVal->setText(m_user.isActive ? "CLEAR / ACTIVE" : "FLAGGED");
    m_statusVal->setStyleSheet(
        m_user.isActive ? "color: #10b981; font-size: 14px; font-weight: 700;"
                        : "color: #ef4444; font-size: 14px; font-weight: 700;"
    );

    if (m_clearanceTitle) {
        m_clearanceTitle->setText(isFaculty ? "Faculty Clearance Level 2 - Authorized"
                                            : "Campus Clearance Level 1 - Validated");
    }
    if (m_clearanceSub) {
        m_clearanceSub->setText(isFaculty ? "Authorized for academic oversight, student concerns, and institutional facilities."
                                          : "All biometric credentials and facility authorizations in good standing.");
    }
}

void StudentProfileCardWidget::onChangePhoto()
{
    const QString file = QFileDialog::getOpenFileName(
        this, "Select Portrait Photo", "", "Image Files (*.png *.jpg *.jpeg *.bmp)"
    );
    if (!file.isEmpty()) {
        if (m_user.role == "faculty") {
            FacultyService::savePhoto(m_user.id, file);
        } else {
            StudentService::savePhoto(m_user.id, file);
        }
        rebuildCard();
        emit profileUpdated(m_user);
    }
}

void StudentProfileCardWidget::onEditInfo()
{
    const bool isFaculty = (m_user.role == "faculty");
    QDialog dlg(this);
    dlg.setWindowTitle(isFaculty ? "Edit Faculty Profile" : "Edit Student Information");
    dlg.setFixedWidth(400);

    auto* form = new QFormLayout(&dlg);
    form->setContentsMargins(20, 20, 20, 20);
    form->setSpacing(12);

    auto* nameEdit = new QLineEdit(m_user.name, &dlg);
    auto* deptEdit = new QLineEdit(m_user.department, &dlg);
    auto* phoneEdit = new QLineEdit(m_user.phone, &dlg);

    auto* bloodEdit = new QComboBox(&dlg);
    bloodEdit->addItems({"Unknown", "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"});
    bloodEdit->setCurrentText(m_user.bloodGroup.isEmpty() ? "Unknown" : m_user.bloodGroup);

    auto* genderBox = new QComboBox(&dlg);
    genderBox->addItems({"Male", "Female", "Other"});
    genderBox->setCurrentText(m_user.gender.isEmpty() ? "Male" : m_user.gender);

    form->addRow("Full Name:", nameEdit);
    form->addRow("Department:", deptEdit);
    form->addRow("Phone:", phoneEdit);
    form->addRow("Blood Group:", bloodEdit);
    form->addRow("Gender:", genderBox);

    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel", &dlg);
    auto* saveBtn = new QPushButton("Save", &dlg);
    saveBtn->setStyleSheet("background: #0288d1; color: white; font-weight: bold; border-radius: 6px; padding: 6px 16px;");
    btnRow->addStretch(1);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(saveBtn);
    form->addRow(btnRow);

    connect(cancelBtn, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, [&]() {
        m_user.name = nameEdit->text().trimmed();
        m_user.department = deptEdit->text().trimmed();
        m_user.phone = phoneEdit->text().trimmed();
        m_user.bloodGroup = bloodEdit->currentText();
        m_user.gender = genderBox->currentText();
        m_user.isActive = true;

        if (isFaculty) {
            FacultyItem f;
            f.facultyId = m_user.id;
            f.name = m_user.name;
            f.department = m_user.department;
            f.phone = m_user.phone;
            f.bloodGroup = m_user.bloodGroup;
            f.gender = m_user.gender;
            f.isActive = true;
            f.designation = m_user.designation;
            FacultyService::update(f);
        } else {
            StudentItem s;
            s.studentId = m_user.id;
            s.name = m_user.name;
            s.department = m_user.department;
            s.phone = m_user.phone;
            s.bloodGroup = m_user.bloodGroup;
            s.gender = m_user.gender;
            s.isActive = true;
            StudentService::update(s);
        }

        dlg.accept();
    });

    if (dlg.exec() == QDialog::Accepted) {
        rebuildCard();
        emit profileUpdated(m_user);
    }
}

// ─────────────────────────────────────────────────────────────
// StudentProfileCardDialog (Opens for Admin / Directory row click)
// ─────────────────────────────────────────────────────────────
StudentProfileCardDialog::StudentProfileCardDialog(const UserProfileData& user, const AppContext& ctx, QWidget* parent)
    : QDialog(parent)
{
    // Exact requirement: window title says simply "Profile"
    setWindowTitle("Profile");
    setFixedWidth(560);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setStyleSheet(
        "QDialog {"
        "  background: #030712;"
        "}"
        "QPushButton#doneBtn {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0288d1, stop:1 #00e5ff);"
        "  color: #030712; border: none; border-radius: 8px; font-size: 13px; font-weight: 700;"
        "  padding: 8px 24px;"
        "}"
        "QPushButton#doneBtn:hover { background: #38bdf8; }"
    );

    auto* vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(16, 16, 16, 16);
    vbox->setSpacing(12);

    m_card = new StudentProfileCardWidget(user, ctx, this);
    connect(m_card, &StudentProfileCardWidget::profileUpdated, this, &StudentProfileCardDialog::profileUpdated);
    vbox->addWidget(m_card, 0, Qt::AlignCenter);

    auto* bottomHBox = new QHBoxLayout;
    bottomHBox->addStretch(1);

    auto* doneBtn = new QPushButton("Done", this);
    doneBtn->setObjectName("doneBtn");
    doneBtn->setCursor(Qt::PointingHandCursor);
    doneBtn->setFixedHeight(36);
    connect(doneBtn, &QPushButton::clicked, this, &QDialog::accept);
    bottomHBox->addWidget(doneBtn);

    vbox->addLayout(bottomHBox);
}

StudentProfileCardDialog::StudentProfileCardDialog(const StudentItem& student, const AppContext& ctx, QWidget* parent)
    : StudentProfileCardDialog(UserProfileData::fromStudent(student), ctx, parent)
{
}

StudentProfileCardDialog::StudentProfileCardDialog(const FacultyItem& faculty, const AppContext& ctx, QWidget* parent)
    : StudentProfileCardDialog(UserProfileData::fromFaculty(faculty), ctx, parent)
{
}
