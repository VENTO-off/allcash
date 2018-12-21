#ifndef ADDADMIN_H
#define ADDADMIN_H

#include <QDialog>

#include "forms/AdminClient.h"

namespace Ui {
    class AddAdmin;
}

class AddAdmin : public QDialog {
    Q_OBJECT

public:
    explicit AddAdmin(AdminClient *adminForm, QWidget *parent = nullptr);
    ~AddAdmin();

private slots:
    void on_loginField_textEdited(const QString &arg1);
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddAdmin *addAdminForm;
    AdminClient *adminForm;

    void applyLocale();
};

#endif
