#include "LoginWindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>
#include <QRegularExpression>
#include <QTextStream>
#include <QVBoxLayout>

#include "services/DataPath.h"

namespace {

const char* kStyle =
    "#card { background: transparent; }"
    "#cardHeader { background: #e3f3f6; border: 1px solid rgba(255,255,255,120); border-bottom: none; }"
    "#cardBody { background: rgba(35,55,75,175); border: 1px solid rgba(255,255,255,120); border-top: none; }"
    "#logoFallback { color: #b71c1c; font-size: 26px; font-weight: 700; background: transparent; }"
    "#fieldIcon { background: rgba(20,38,56,215); border: 1px solid rgba(255,255,255,90); color: white; font-size: 20px; }"
    "#field { background: white; border: 1px solid #9ec9e8; }"
    "#field QLineEdit { background: transparent; border: none; color: #222; font-size: 17px; }"
    "#eyeBtn { border: none; background: transparent; font-size: 18px; padding: 0 6px; }"
    "#eyeBtn:checked { background: #e3f3f6; }"
    "#loginBtn { background: #03a9f4; color: white; border: none; font-size: 17px; font-weight: 700; }"
    "#loginBtn:hover { background: #29b6f6; }"
    "#loginBtn:pressed { background: #0288d1; }"
    "#errorLabel { color: #ffb4b4; font-size: 14px; background: transparent; }";

// users.txt sits in "data" next to exe, or project root
QString findUsersFile()
{
    return DataPath::resolve("users.txt");
}

// users.txt lines: "<id> <role> [admin password]"
bool lookupUser(const QString& id, QString& role, QString& password)
{
    QFile f(findUsersFile());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QTextStream in(&f);
    while (!in.atEnd()) {
        const QStringList t = in.readLine().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (t.size() >= 2 && t[0] == id) {
            role = t[1];
            password = t.size() > 2 ? t[2] : QString();
            return true;
        }
    }
    return false;
}

} // namespace

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent), m_bg(":/campus_bg.jpg")
{
    setWindowTitle("CampusCare - Login");
    setMinimumSize(1024, 640);
    resize(1280, 720);
    setStyleSheet(kStyle);

    auto* outer = new QVBoxLayout(this);
    outer->setAlignment(Qt::AlignCenter);

    auto* card = new QFrame;
    card->setObjectName("card");
    card->setFixedWidth(460);
    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(0, 0, 0, 0);
    cardLayout->setSpacing(0);

    auto* header = new QFrame;
    header->setObjectName("cardHeader");
    auto* hl = new QVBoxLayout(header);
    hl->setContentsMargins(24, 28, 24, 28);
    auto* logo = new QLabel;
    logo->setAlignment(Qt::AlignCenter);
    const QPixmap lp(":/logo.png");
    if (!lp.isNull()) {
        logo->setPixmap(lp.scaledToWidth(360, Qt::SmoothTransformation));
    } else {
        logo->setText("Graphic Era Hill University");
        logo->setObjectName("logoFallback");
    }
    hl->addWidget(logo);
    cardLayout->addWidget(header);

    auto* body = new QFrame;
    body->setObjectName("cardBody");
    auto* bl = new QVBoxLayout(body);
    bl->setContentsMargins(36, 32, 36, 32);
    bl->setSpacing(14);

    bl->addWidget(makeRow(QString::fromUtf8("\xF0\x9F\x91\xA4"), m_idEdit, "User ID", false));
    m_passRow = makeRow(QString::fromUtf8("\xF0\x9F\x94\x91"), m_passEdit, "Admin password", true);
    m_passRow->setVisible(false);
    bl->addWidget(m_passRow);

    m_loginBtn = new QPushButton("LOGIN");
    m_loginBtn->setObjectName("loginBtn");
    m_loginBtn->setCursor(Qt::PointingHandCursor);
    m_loginBtn->setFixedHeight(48);
    bl->addWidget(m_loginBtn);

    m_error = new QLabel;
    m_error->setObjectName("errorLabel");
    m_error->setWordWrap(true);
    m_error->setAlignment(Qt::AlignCenter);
    m_error->hide();
    bl->addWidget(m_error);

    cardLayout->addWidget(body);
    outer->addWidget(card);

    connect(m_idEdit, &QLineEdit::textChanged, this, &LoginWindow::onIdChanged);
    connect(m_idEdit, &QLineEdit::returnPressed, this, [this] {
        if (m_passRow->isVisible()) m_passEdit->setFocus();
        else attemptLogin();
    });
    connect(m_passEdit, &QLineEdit::returnPressed, this, &LoginWindow::attemptLogin);
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
}

QWidget* LoginWindow::makeRow(const QString& icon, QLineEdit*& edit,
                              const QString& placeholder, bool eye)
{
    auto* row = new QWidget;
    auto* h = new QHBoxLayout(row);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);

    auto* ic = new QLabel(icon);
    ic->setObjectName("fieldIcon");
    ic->setAlignment(Qt::AlignCenter);
    ic->setFixedSize(76, 52);

    auto* field = new QFrame;
    field->setObjectName("field");
    field->setFixedHeight(52);
    auto* fl = new QHBoxLayout(field);
    fl->setContentsMargins(16, 0, 8, 0);
    fl->setSpacing(0);

    edit = new QLineEdit;
    edit->setPlaceholderText(placeholder);
    fl->addWidget(edit);

    if (eye) {
        edit->setEchoMode(QLineEdit::Password);
        auto* b = new QPushButton(QString::fromUtf8("\xF0\x9F\x91\x81"));
        b->setObjectName("eyeBtn");
        b->setCheckable(true);
        b->setCursor(Qt::PointingHandCursor);
        connect(b, &QPushButton::toggled, this, [edit](bool on) {
            edit->setEchoMode(on ? QLineEdit::Normal : QLineEdit::Password);
        });
        fl->addWidget(b);
    }

    h->addWidget(ic);
    h->addWidget(field, 1);
    return row;
}

void LoginWindow::onIdChanged(const QString& text)
{
    m_error->hide();
    QString role, pw;
    const bool admin = lookupUser(text.trimmed(), role, pw) && role == "admin";
    m_passRow->setVisible(admin);
    if (!admin) m_passEdit->clear();
}

void LoginWindow::attemptLogin()
{
    const QString id = m_idEdit->text().trimmed();
    if (id.isEmpty()) { showError("Enter your User ID."); return; }
    if (!QFile::exists(findUsersFile())) { showError("users.txt not found next to the app."); return; }

    QString role, pw;
    if (!lookupUser(id, role, pw)) { showError("This ID is not registered."); return; }

    // Plain comparison, same as the console app. Swap in libsodium verify here later.
    if (role == "admin" && m_passEdit->text() != pw) { showError("Incorrect admin password."); return; }

    emit loggedIn(id, role);
}

void LoginWindow::showError(const QString& msg)
{
    m_error->setText(msg);
    m_error->show();
}

void LoginWindow::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    if (!m_bg.isNull())
        m_scaled = m_bg.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
}

void LoginWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (!m_scaled.isNull())
        p.drawPixmap((width() - m_scaled.width()) / 2, (height() - m_scaled.height()) / 2, m_scaled);
    p.fillRect(rect(), QColor(0, 0, 0, 70));
}
