#ifndef DEFAULTUSER_H
#define DEFAULTUSER_H

#include "User.h"

class DefaultUser : public User {
private:
    long regDate;
    double balance;
    QString walletId;
public:
    DefaultUser();
    DefaultUser(int id, QString login, QString encryptedPassword, QString firstName, QString secondName, QString middleName, long regDate, double balance, QString walletId);
    long getRegDate();
    double getBalance();
    void consumeMoney(double amount);
    QString getWalletId();
    void logIn();
    void logOut();
};

#endif
