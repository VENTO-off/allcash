#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

namespace Ui {
    class ChangePassword;
}

class ChangePassword : public QDialog {
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = nullptr);
    ~ChangePassword();

private slots:
    void on_changePasswordButton_clicked();
    void on_oldPasswordField_textEdited(const QString &arg1);
    void on_newPasswordField_textEdited(const QString &arg1);
    void on_newPassword2Field_textEdited(const QString &arg1);

private:
    Ui::ChangePassword *changePasswordForm;

    void applyLocale();
    void checkFields();
};

#endif
