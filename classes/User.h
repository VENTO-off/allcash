#ifndef USER_H
#define USER_H

#include <QString>

#include <managers/SettingsManager.h>

extern SettingsManager settingsManager;

class User {
protected:
    int id;
    QString login;
    QString encryptedPassword;
    QString firstName;
    QString secondName;
    QString middleName;
public:
    User();
    User(int id, QString login, QString encryptedPassword, QString firstName, QString secondName, QString middleName);
    QString getLogin();
    QString getEncryptedPassword();
    void changeEncryptedPassword(QString encryptedPassword);
    QString getFirstName();
    QString getSecondName();
    QString getMiddleName();
    virtual void logIn() = 0;
    virtual void logOut() = 0;
};

#endif
