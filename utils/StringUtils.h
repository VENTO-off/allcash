#ifndef PASSWORDUTILS_H
#define PASSWORDUTILS_H

#include <QString>
#include <QRegularExpression>
#include <vector>

using std::vector;

namespace StringUtils {
    bool isPasswordValid(QString password);
    int getCoins(double amount);
    QString formatAmount(double amount);
    bool isAmountValid(QString s_amount);
    bool isWalletValid(QString walletId);
    bool isPhoneValid(QString phone);
    bool isAdminLoginValid(QString login);
    bool isPersonalNameValid(QString name);
    QString removeHtmlTags(QString value);
}

#endif
