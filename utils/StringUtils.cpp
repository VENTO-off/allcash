#include "utils/StringUtils.h"

namespace StringUtils {
    bool isPasswordValid(QString password) {
        if (password.length() < 6 || password.length() > 32) {
            return false;
        }

        QRegularExpression passwordRegex("(?=.*[[:upper:]])(?=.*[[:lower:]])(?=.*[0-9])");
        if (!passwordRegex.match(password).hasMatch()) {
            return false;
        }

        return true;
    }

    int getCoins(double amount) {
        return QString::number(amount, 'f', 2).split('.').at(1).toInt();
    }

    QString formatAmount(double amount) {
        return QString::number(amount, 'f', 2);
    }

    bool isAmountValid(QString s_amount) {
        bool isValid;
        double amount = s_amount.toDouble(&isValid);

        if (!isValid) {
            return false;
        }

        int coins = getCoins(amount);

        if (coins > 99) {
            return false;
        }

        return true;
    }

    bool isWalletValid(QString walletId) {
        if (walletId.length() != 10) {
            return false;
        }

        bool isValid;
        walletId.toLong(&isValid);

        if (!isValid) {
            return false;
        }

        return true;
    }

    bool isPhoneValid(QString phone) {
        if (phone.length() != 13) {
            return false;
        }

        QRegularExpression phoneRegex("^[+]{1}[0-9]*$");
        if (!phoneRegex.match(phone).hasMatch()) {
            return false;
        }

        return true;
    }

    bool isAdminLoginValid(QString login) {
        if (login.isEmpty()) {
            return false;
        }

        if (login.length() > 32) {
            return false;
        }

        if (login == "admin_") {
            return false;
        }

        QRegularExpression adminRegex("^admin_[A-Za-z0-9]*$");
        if (!adminRegex.match(login).hasMatch()) {
            return false;
        }

        return true;
    }

    bool isPersonalNameValid(QString name) {
        if (name.isEmpty()) {
            return false;
        }

        if (name.length() > 40) {
            return false;
        }

        QRegularExpression personalNameRegex("^[A-ZА-ЯҐЄІЇ]{1}[A-ZА-ЯҐЄІЇa-zа-яґєії-]*$");
        if (!personalNameRegex.match(name).hasMatch()) {
            return false;
        }

        return true;
    }

    QString removeHtmlTags(QString value) {
        return value.remove(QRegExp("<[^>]*>"));
    }
}
