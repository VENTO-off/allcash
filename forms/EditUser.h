#ifndef EDITUSER_H
#define EDITUSER_H

#include <QDialog>

#include "forms/AdminClient.h"

#include "classes/DefaultUser.h"

namespace Ui {
    class EditUser;
}

class EditUser : public QDialog {
    Q_OBJECT

public:
    explicit EditUser(AdminClient *adminForm, DefaultUser editableUser, int id, QWidget *parent = nullptr);
    ~EditUser();

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::EditUser *editUserForm;
    AdminClient *adminForm;
    DefaultUser editableUser;
    int id;

    void applyLocale();
    void loadData();
};

#endif
