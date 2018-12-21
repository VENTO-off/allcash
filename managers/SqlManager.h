#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>

#include "SqlConfig.h"

#include "classes/AdminUser.h"
#include "classes/User.h"
#include "classes/DefaultUser.h"

using std::map;
using std::pair;

class SqlManager {
private:
    QSqlDatabase db;
public:
    SqlManager();
    void connect();
    void disconnect();
    bool checkVersion();
    bool isUserExists(QString login);
    QString getUserPersonalData(QString walletId);
    bool isWalletExists(QString walletId);
    User* authUser(QString login, QString password);
    QString updateLastWalletId();
    void registerUser(QString login, QString password, QString firstName, QString secondName, QString middleName);
    void changePassword(QString login, QString password);
    void sendMoney(double amount, QString fromWallet, QString toWallet);
    map<int, DefaultUser> loadUsers();
    map<int, AdminUser> loadAdmins();
    void editUser(DefaultUser editableUser, int id, QString phone, QString password, double QSmount, QString firstName, QString secondName, QString middleName);
    bool isAdminExists(QString login);
    void registerAdmin(QString login, QString password, QString firstName, QString secondName, QString middleName, bool addAdmin, bool editPhone, bool editPassword, bool editPersonalData, bool editBalance);
    void deleteAdmin(QString login);
};

#endif
