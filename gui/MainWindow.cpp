// ─────────────────────────────────────────────────────────────
// CampusCare GUI — Main Application Window (Implementation)
// ─────────────────────────────────────────────────────────────

#include "MainWindow.h"
#include "theme/ThemeManager.h"
#include "widgets/BasePage.h"

#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

// ─────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────
MainWindow::MainWindow(const AppContext& ctx, QWidget* parent)
    : QMainWindow(parent), m_ctx(ctx)
{
    setWindowTitle(QString::fromUtf8("CampusCare \u2014 Student Safety & Emergency Response"));
    setMinimumSize(1024, 640);
    resize(1280, 720);

    // Filter nav entries for this role
    const auto all = buildNavRegistry();
    for (const auto& e : all) {
        if (e.roles.contains(m_ctx.role))
            m_nav.append(e);
    }
    m_pages.resize(m_nav.size(), nullptr);

    // Central container
    auto* central = new QWidget(this);
    central->setObjectName("centralContainer");
    auto* hbox = new QHBoxLayout(central);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);

    // Sidebar
    buildSidebar();
    hbox->addWidget(m_sidebar);

    // Right side: top bar + content stack
    auto* rightSide = new QWidget;
    auto* rightLayout = new QVBoxLayout(rightSide);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    buildTopBar();
    auto* topBar = m_pageTitle->parentWidget();
    rightLayout->addWidget(topBar);

    m_stack = new QStackedWidget;
    m_stack->setObjectName("contentArea");
    rightLayout->addWidget(m_stack, 1);

    hbox->addWidget(rightSide, 1);
    setCentralWidget(central);

    // Select first nav item
    if (!m_navBtns.isEmpty()) {
        m_navBtns[0]->setChecked(true);
        onNavClicked(0);
    }
}

// ─────────────────────────────────────────────────────────────
// Sidebar
// ─────────────────────────────────────────────────────────────
void MainWindow::buildSidebar()
{
    m_sidebar = new QWidget;
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFixedWidth(230);

    auto* vbox = new QVBoxLayout(m_sidebar);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    // Brand
    auto* brand = new QLabel(QString::fromUtf8("\xF0\x9F\x9B\xA1  CampusCare"));
    brand->setObjectName("sidebarBrand");
    vbox->addWidget(brand);

    auto* brandSub = new QLabel("Safety & Emergency Response");
    brandSub->setObjectName("sidebarBrandSub");
    vbox->addWidget(brandSub);

    // Separator
    auto* sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color: #2a3444; max-height: 1px;");
    vbox->addWidget(sep);

    // Nav buttons — plain widget (no QScrollArea to avoid layout oscillation)
    auto* navContainer = new QWidget;
    navContainer->setObjectName("navContainer");
    navContainer->setStyleSheet("background: transparent;");
    auto* navLayout = new QVBoxLayout(navContainer);
    navLayout->setContentsMargins(0, 8, 0, 8);
    navLayout->setSpacing(2);

    m_navGroup = new QButtonGroup(this);
    m_navGroup->setExclusive(true);

    for (int i = 0; i < m_nav.size(); ++i) {
        auto* btn = new QPushButton(m_nav[i].label);
        btn->setObjectName("navBtn");
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedSize(230, 36);
        m_navGroup->addButton(btn, i);
        m_navBtns.append(btn);
        navLayout->addWidget(btn);
    }
    navLayout->addStretch(1);
    vbox->addWidget(navContainer, 1);

    connect(m_navGroup, &QButtonGroup::idClicked,
            this, &MainWindow::onNavClicked);

    // Separator before user card
    auto* sep2 = new QFrame;
    sep2->setFrameShape(QFrame::HLine);
    sep2->setStyleSheet("color: #2a3444; max-height: 1px;");
    vbox->addWidget(sep2);

    // User card
    auto* card = new QFrame;
    card->setObjectName("userCard");
    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(14, 10, 14, 10);
    cardLayout->setSpacing(2);

    auto* userId = new QLabel(m_ctx.userId);
    userId->setObjectName("userCardId");
    cardLayout->addWidget(userId);

    auto* userRole = new QLabel(m_ctx.role.toUpper());
    userRole->setObjectName("userCardRole");
    cardLayout->addWidget(userRole);
    vbox->addWidget(card);

    // Logout button
    auto* logoutBtn = new QPushButton(QString::fromUtf8("  \xE2\x86\x90  Logout"));
    logoutBtn->setObjectName("logoutBtn");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setFixedHeight(36);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogout);
    vbox->addWidget(logoutBtn);
}

// ─────────────────────────────────────────────────────────────
// Top bar
// ─────────────────────────────────────────────────────────────
void MainWindow::buildTopBar()
{
    auto* bar = new QWidget;
    bar->setObjectName("topBar");
    bar->setFixedHeight(56);

    auto* hbox = new QHBoxLayout(bar);
    hbox->setContentsMargins(24, 0, 24, 0);
    hbox->setSpacing(12);

    m_pageTitle = new QLabel;
    m_pageTitle->setObjectName("pageTitle");
    hbox->addWidget(m_pageTitle);

    hbox->addStretch(1);

    m_userChip = new QLabel(m_ctx.userId + "  \xE2\x80\xA2  " + m_ctx.role);
    m_userChip->setObjectName("userChip");
    hbox->addWidget(m_userChip);

    const bool isDark = ThemeManager::instance().currentTheme() == ThemeManager::Dark;
    m_themeBtn = new QPushButton(isDark ? QString::fromUtf8("\xF0\x9F\x8C\x99")
                                       : QString::fromUtf8("\xE2\x98\x80"));
    m_themeBtn->setObjectName("themeToggleBtn");
    m_themeBtn->setFixedSize(36, 36);
    m_themeBtn->setCursor(Qt::PointingHandCursor);
    m_themeBtn->setToolTip("Toggle Light / Dark Theme");
    connect(m_themeBtn, &QPushButton::clicked, this, &MainWindow::onThemeToggle);
    hbox->addWidget(m_themeBtn);
}

// ─────────────────────────────────────────────────────────────
// Lazy page creation
// ─────────────────────────────────────────────────────────────
QWidget* MainWindow::getOrCreatePage(int index)
{
    if (index < 0 || index >= m_nav.size()) return nullptr;
    if (!m_pages[index]) {
        m_pages[index] = m_nav[index].factory(m_stack, m_ctx);
        m_stack->addWidget(m_pages[index]);
    }
    return m_pages[index];
}

// ─────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────
void MainWindow::onNavClicked(int index)
{
    auto* page = getOrCreatePage(index);
    if (!page) return;
    m_stack->setCurrentWidget(page);

    // Update page title
    auto* bp = qobject_cast<BasePage*>(page);
    m_pageTitle->setText(bp ? bp->title() : m_nav[index].label);

    // Refresh data
    if (bp) bp->refresh();
}

void MainWindow::onThemeToggle()
{
    ThemeManager::instance().toggleTheme();
    const bool isDark = ThemeManager::instance().currentTheme() == ThemeManager::Dark;
    m_themeBtn->setText(isDark ? QString::fromUtf8("\xF0\x9F\x8C\x99")
                               : QString::fromUtf8("\xE2\x98\x80"));
}

void MainWindow::onLogout()
{
    emit logoutRequested();
}
