#include "classes/User.h"

#include "managers/MoneyManager.h"
#include "managers/LogManager.h"
#include "managers/SqlManager.h"
#include "managers/NotifyManager.h"
#include "managers/LocaleManager.h"

#include "utils/TimeUtils.h"

using TimeUtils::getCurrentTimestamp;

extern User *loggedAccount;
extern SqlManager sqlManager;
extern LocaleManager localeManager;

MoneyManager::MoneyManager() {}

bool MoneyManager::hasMoney(double amount) {
    return static_cast<DefaultUser*>(loggedAccount)->getBalance() >= amount;
}

void MoneyManager::sendMoney(double amount, QString toWallet) {
    if (!hasMoney(amount)) {
        throw "msg.money.notenoughmoney";
    }

    if (!sqlManager.isWalletExists(toWallet)) {
        throw "msg.general.walletnotexists";
    }

    //Transfer money
    try {
        sqlManager.sendMoney(amount, static_cast<DefaultUser*>(loggedAccount)->getWalletId(), toWallet);
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    //Update logs
    LogManager senderLogs = LogManager(static_cast<DefaultUser*>(loggedAccount)->getWalletId());
    senderLogs.readLogs();
    senderLogs.addLog(TransactionLog(CONSUME, getCurrentTimestamp(), "%transfer_to% " + toWallet + ".", amount));
    try {
        senderLogs.writeLogs();
    } catch (const char* exception) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate(exception));
    }
    static_cast<DefaultUser*>(loggedAccount)->consumeMoney(amount);

    //Update logs
    LogManager recipientLogs = LogManager(toWallet);
    recipientLogs.readLogs();
    recipientLogs.addLog(TransactionLog(INCOME, getCurrentTimestamp(), "%transfer_from% " + static_cast<DefaultUser*>(loggedAccount)->getWalletId() + ".", amount));
    try {
        recipientLogs.writeLogs();
    } catch (const char* exception) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate(exception));
    }
}
