#ifndef MONEYMANAGER_H
#define MONEYMANAGER_H

#include <QString>

class MoneyManager {
public:
    MoneyManager();
    bool hasMoney(double amount);
    void sendMoney(double amount, QString toWallet);
};

#endif
