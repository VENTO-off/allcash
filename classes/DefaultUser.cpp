#include "DefaultUser.h"

#include "forms/Auth.h"
#include "forms/UserClient.h"

DefaultUser::DefaultUser() {}

DefaultUser::DefaultUser(int id, QString login, QString password, QString firstName, QString secondName, QString middleName, long regDate, double balance, QString walletId) :
    User(id, login, password, firstName, secondName, middleName) {
    this->regDate = regDate;
    this->balance = balance;
    this->walletId = walletId;
}

long DefaultUser::getRegDate() {
    return this->regDate;
}

double DefaultUser::getBalance() {
    return this->balance;
}

void DefaultUser::consumeMoney(double amount) {
    this->balance -= amount;
}

QString DefaultUser::getWalletId() {
    return this->walletId;
}

void DefaultUser::logIn() {
    UserClient *userClient = new UserClient();
    userClient->show();

    settingsManager.setLastAccount(getLogin());
    settingsManager.save();
}

void DefaultUser::logOut() {
    Auth *auth = new Auth(getLogin());
    auth->show();
}

