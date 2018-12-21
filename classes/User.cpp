#include "User.h"

User::User() {}

User::User(int id, QString login, QString password, QString firstName, QString secondName, QString middleName) {
    this->id = id;
    this->login = login;
    this->encryptedPassword = password;
    this->firstName = firstName;
    this->secondName = secondName;
    this->middleName = middleName;
}

QString User::getLogin() {
    return this->login;
}

QString User::getEncryptedPassword() {
    return this->encryptedPassword;
}

void User::changeEncryptedPassword(QString encryptedPassword) {
    this->encryptedPassword = encryptedPassword;
}

QString User::getFirstName() {
    return this->firstName;
}

QString User::getSecondName() {
    return this->secondName;
}

QString User::getMiddleName() {
    return this->middleName;
}
