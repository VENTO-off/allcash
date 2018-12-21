#ifndef ADMINCLIENT_H
#define ADMINCLIENT_H

#include <QWidget>

#include "classes/AdminUser.h"
#include "classes/DefaultUser.h"

using std::map;
using std::distance;

namespace Ui {
    class AdminClient;
}

class AdminClient : public QWidget {
    Q_OBJECT

public:
    explicit AdminClient(QWidget *parent = nullptr);
    void updateData();
    ~AdminClient();

private slots:
    void onEditUserClicked();
    void onDeleteAdminClicked();
    void on_changePasswordButton_clicked();
    void on_logOutButton_clicked();
    void on_findUserField_textEdited(const QString &arg1);
    void on_findAdminField_textEdited(const QString &arg1);
    void on_addAdminButton_clicked();
    void on_helpButton_clicked();

private:
    Ui::AdminClient *adminForm;
    map<int, DefaultUser> users;
    map<int, AdminUser> admins;

    void applyLocale();
    void initIcons();
    void updateUsers(map<int, DefaultUser> users);
    void updateAdmins(map<int, AdminUser> admins);
};

#endif
