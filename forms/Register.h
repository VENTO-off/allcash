#ifndef REGISTER_H
#define REGISTER_H

#include <QLabel>
#include <QWidget>

namespace Ui {
    class Register;
}

class Register : public QWidget {
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private slots:
    void Mouse_Pressed();
    void on_loginField_textEdited(const QString &arg1);
    void on_loginField_editingFinished();
    void on_passwordField_textEdited(const QString &arg1);
    void on_passwordField_editingFinished();
    void on_password2Field_textEdited(const QString &arg1);
    void on_password2Field_editingFinished();
    void on_firstNameField_textEdited(const QString &arg1);
    void on_firstNameField_editingFinished();
    void on_secondNameField_textEdited(const QString &arg1);
    void on_secondNameField_editingFinished();
    void on_middleNameField_textEdited(const QString &arg1);
    void on_middleNameField_editingFinished();
    void on_registerButton_clicked();
    void on_helpButton_clicked();

private:
    Ui::Register *registerForm;

    void applyLocale();
    void initIcons();
    void checkFields();
    void setSuccess(QLabel *status);
    void setError(QLabel *status);
};

#endif
