#ifndef ADMINUSER_H
#define ADMINUSER_H

#include "User.h"

class AdminUser : public User {
private:
    bool addAdmin;
    bool editPhone;
    bool editPassword;
    bool editPersonalData;
    bool editBalance;
public:
    AdminUser();
    AdminUser(int id, QString login, QString encryptedPassword, QString firstName, QString secondName, QString middleName, bool addAdmin, bool editPhone, bool editPassword, bool editPersonalData, bool editBalance);
    bool canAddAdmin();
    bool canEditPhone();
    bool canEditPassword();
    bool canEditPersonalData();
    bool canEditBalance();
    void logIn();
    void logOut();
};

#endif
