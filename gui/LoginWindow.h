#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QPointF>
#include <QVector>

class QLineEdit;
class QLabel;
class QPushButton;
class QTimer;
class QVBoxLayout;
class QScrollArea;

struct LoginAlertItem {
    int incidentId = 0;
    QString icon;
    QString category;
    QString location;
    QString type;
    QString description;
    QString reporter;
    QString status;
    bool expanded = false;
};

class LoginWindow : public QWidget {
    Q_OBJECT
    Q_PROPERTY(double introProgress READ introProgress WRITE setIntroProgress)
    Q_PROPERTY(double cardAnimProgress READ cardAnimProgress WRITE setCardAnimProgress)

public:
    explicit LoginWindow(QWidget* parent = nullptr);

    double introProgress() const { return m_introProgress; }
    void setIntroProgress(double val);

    double cardAnimProgress() const { return m_cardAnimProgress; }
    void setCardAnimProgress(double val);

signals:
    void loggedIn(const QString& id, const QString& role);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onIdChanged(const QString& text);
    void attemptLogin();
    void onBgTimerTick();
    void skipIntro();
    void fillDemoAccount(const QString& id, const QString& pass = QString());

private:
    enum Stage {
        StageIntro,
        StageLogin
    };

    void setupUI();
    void buildAlertPanel();
    void repositionAlertPanel();
    QWidget* makeRow(const QString& icon, QLineEdit*& edit,
                     const QString& placeholder, bool eye);
    void showError(const QString& msg);
    void updateRoleBadge(const QString& text);

    Stage m_stage = StageIntro;
    double m_introProgress = 0.0;
    double m_cardAnimProgress = 0.0;
    int m_introPulseTick = 0;

    // Stable background caching & float panning
    QPixmap m_bg;
    QPixmap m_scaledBg;
    QTimer* m_bgTimer = nullptr;
    double m_bgPanAngle = 0.0;
    QPointF m_mouseOffset;
    QPointF m_targetMouseOffset;

    // UI Widgets
    QWidget*     m_cardWidget     = nullptr;
    QLineEdit*   m_idEdit         = nullptr;
    QLineEdit*   m_passEdit       = nullptr;
    QWidget*     m_passRow        = nullptr;
    QPushButton* m_loginBtn       = nullptr;
    QLabel*      m_error          = nullptr;
    QLabel*      m_roleBadge      = nullptr;
    QPushButton* m_skipBtn        = nullptr;

    // Alert panel (top-right corner)
    QWidget*      m_alertPanel    = nullptr;
    QScrollArea*  m_alertScroll   = nullptr;
    QWidget*      m_alertContent  = nullptr;
    QVBoxLayout*  m_alertLayout   = nullptr;
    QVector<LoginAlertItem> m_alerts;
};

#endif // LOGINWINDOW_H
