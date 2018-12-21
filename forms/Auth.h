#ifndef AUTH_H
#define AUTH_H

#include <QMainWindow>

namespace Ui {
    class Auth;
}

class Auth : public QMainWindow {
    Q_OBJECT

public:
    explicit Auth(QString login = "", QWidget *parent = nullptr);
    ~Auth();

private slots:
    void Mouse_Pressed();
    void on_authButton_clicked();
    void on_loginField_textEdited(const QString &arg1);
    void on_passwordField_textEdited(const QString &arg1);
    void on_helpButton_clicked();
    void on_developersButton_clicked();

private:
    Ui::Auth *authForm;

    void applyLocale();
    void initIcons();
    void checkFields();
};

#endif
