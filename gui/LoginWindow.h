#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPixmap>

class QLineEdit;
class QLabel;
class QPushButton;

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = nullptr);

signals:
    void loggedIn(const QString& id, const QString& role);

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private slots:
    void onIdChanged(const QString& text);
    void attemptLogin();

private:
    QWidget* makeRow(const QString& icon, QLineEdit*& edit,
                     const QString& placeholder, bool eye);
    void showError(const QString& msg);

    QPixmap m_bg, m_scaled;
    QLineEdit*   m_idEdit    = nullptr;
    QLineEdit*   m_passEdit  = nullptr;
    QWidget*     m_passRow   = nullptr;
    QPushButton* m_loginBtn  = nullptr;
    QLabel*      m_error     = nullptr;
};

#endif // LOGINWINDOW_H
