#include "AdminUser.h"

#include "forms/AdminClient.h"
#include "forms/Auth.h"

AdminUser::AdminUser() {}

AdminUser::AdminUser(int id, QString login, QString password, QString firstName, QString secondName, QString middleName, bool addAdmin, bool editPhone, bool editPassword, bool editPersonalData, bool editBalance) :
    User(id, login, password, firstName, secondName, middleName) {
    this->addAdmin = addAdmin;
    this->editPhone = editPhone;
    this->editPassword = editPassword;
    this->editPersonalData = editPersonalData;
    this->editBalance = editBalance;
}

bool AdminUser::canAddAdmin() {
    return this->addAdmin;
}

bool AdminUser::canEditPhone() {
    return this->editPhone;
}

bool AdminUser::canEditPassword() {
    return this->editPassword;
}

bool AdminUser::canEditPersonalData() {
    return this->editPersonalData;
}

bool AdminUser::canEditBalance() {
    return this->editBalance;
}

void AdminUser::logIn() {
    AdminClient *userClient = new AdminClient();
    userClient->show();

    settingsManager.setLastAccount("admin_");
    settingsManager.save();
}

void AdminUser::logOut() {
    Auth *auth = new Auth("admin_");
    auth->show();
}
