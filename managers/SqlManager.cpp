#include "classes/AdminUser.h"

#include "SqlManager.h"

#include "managers/NotifyManager.h"

#include "utils/TimeUtils.h"

#include "libs/md5/md5.h"
#include "libs/base64/base64.h"

using TimeUtils::getCurrentTimestamp;
using std::cerr;
using std::endl;

SqlManager::SqlManager() {}

void SqlManager::connect() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(DB_HOST);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    db.setPassword(QString::fromStdString(base64_decode(DB_PASS)));

    db.open();

    if (db.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (db.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << db.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }
}

void SqlManager::disconnect() {
    db.close();
}

bool SqlManager::checkVersion() {
    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_SYSTEM + "` WHERE `key` = '" + SYSTEM_LAST_VERSION + "'");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    while (query.next()) {
        QString version =  query.value("value").toString().toUtf8().constData();
        if (version != VERSION) {
            return false;
        }
    }

    return true;
}

bool SqlManager::isUserExists(QString login) {
    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_USERS + "` WHERE `" + USER_PHONE + "` = '" + login + "'");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    if (query.size() == 0) {
        return false;
    }

    return true;
}

QString SqlManager::getUserPersonalData(QString walletId) {
    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_USERS + "` WHERE `" + USER_WALLET_ID + "` = '" + walletId + "'");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    while (query.next()) {
        QString firstName = query.value(USER_FIRST_NAME).toString().toUtf8().constData();
        QString secondName = query.value(USER_SECOND_NAME).toString().toUtf8().constData();
        QString middleName = query.value(USER_MIDDLE_NAME).toString().toUtf8().constData();

        return secondName + " " + firstName.mid(0, 1) + "." + " " + middleName.mid(0, 1) + ".";
    }

    return 0;
}

bool SqlManager::isWalletExists(QString walletId) {
    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_USERS + "` WHERE `" + USER_WALLET_ID + "` = '" + walletId + "'");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    if (query.size() == 0) {
        return false;
    }

    return true;
}

User* SqlManager::authUser(QString login, QString password) {
    QSqlQuery query(db);
    if (login.startsWith("admin_")) {
        query.exec(QString() + "SELECT * FROM `" + TABLE_ADMINS + "` WHERE `" + ADMIN_LOGIN + "` = '" + login + "'");

        if (query.lastError().type() == QSqlError::ConnectionError) {
            throw "msg.general.nointernet";
        } else if (query.lastError().type() == QSqlError::StatementError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.statementerror";
        } else if (query.lastError().type() == QSqlError::UnknownError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.unknownerror";
        }

        while (query.next()) {
            QString encryptedPassword = query.value(ADMIN_PASSWORD).toString().toUtf8().constData();
            if (md5(md5(password.toStdString())) == encryptedPassword.toStdString()) {
                int id = query.value(ADMIN_ID).toInt();
                QString login = query.value(ADMIN_LOGIN).toString().toUtf8().constData();
                QString firstName = query.value(ADMIN_FIRST_NAME).toString().toUtf8().constData();
                QString secondName = query.value(ADMIN_SECOND_NAME).toString().toUtf8().constData();
                QString middleName = query.value(ADMIN_MIDDLE_NAME).toString().toUtf8().constData();
                bool addAdmin = query.value(ADMIN_CAN_ADD_ADMIN).toBool();
                bool editPhone = query.value(ADMIN_CAN_EDIT_PHONE).toBool();
                bool editPassword = query.value(ADMIN_CAN_EDIT_PASSWORD).toBool();
                bool editPersonalData = query.value(ADMIN_CAN_EDIT_PERSONAL_DATA).toBool();
                bool editBalance = query.value(ADMIN_CAN_EDIT_BALANCE).toBool();

                return new AdminUser(id, login, encryptedPassword, firstName, secondName, middleName, addAdmin, editPhone, editPassword, editPersonalData, editBalance);
            }
        }
    } else {
        query.exec(QString() + "SELECT * FROM `" + TABLE_USERS + "` WHERE `" + USER_PHONE + "` = '" + login + "'");

        if (query.lastError().type() == QSqlError::ConnectionError) {
            throw "msg.general.nointernet";
        } else if (query.lastError().type() == QSqlError::StatementError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.statementerror";
        } else if (query.lastError().type() == QSqlError::UnknownError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.unknownerror";
        }

        while (query.next()) {
            QString encryptedPassword = query.value(USER_PASSWORD).toString().toUtf8().constData();
            if (md5(md5(password.toStdString())) == encryptedPassword.toStdString()) {
                int id = query.value(USER_ID).toInt();
                QString login = query.value(USER_PHONE).toString().toUtf8().constData();
                QString firstName = query.value(USER_FIRST_NAME).toString().toUtf8().constData();
                QString secondName = query.value(USER_SECOND_NAME).toString().toUtf8().constData();
                QString middleName = query.value(USER_MIDDLE_NAME).toString().toUtf8().constData();
                long regDate = query.value(USER_REG_DATE).toLongLong();
                double balance = query.value(USER_BALANCE).toDouble();
                QString walletId = query.value(USER_WALLET_ID).toString().toUtf8().constData();

                return new DefaultUser(id, login, encryptedPassword, firstName, secondName, middleName, regDate, balance, walletId);
            }
        }  
    }

    return 0;
}

QString SqlManager::updateLastWalletId() {
    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_SYSTEM + "` WHERE `key` = '" + SYSTEM_LAST_WALLET_ID + "'");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    if (query.size() == 0) {
        QString last_wallet_id;
        last_wallet_id.fill('0', 9);
        last_wallet_id.append('1');
        query.exec(QString() + "INSERT INTO `" + TABLE_SYSTEM + "` (`key`, `value`) VALUES ('" + SYSTEM_LAST_WALLET_ID + "', '" + last_wallet_id + "')");

        if (query.lastError().type() == QSqlError::ConnectionError) {
            throw "msg.general.nointernet";
        } else if (query.lastError().type() == QSqlError::StatementError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.statementerror";
        } else if (query.lastError().type() == QSqlError::UnknownError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.unknownerror";
        }

        return last_wallet_id;
    }

    while (query.next()) {
        QString last_wallet_id = query.value("value").toString().toUtf8().constData();
        QString new_wallet_id = QString::number(last_wallet_id.toLong() + 1);
        last_wallet_id.fill('0', 10 - new_wallet_id.length()).append(new_wallet_id);
        query.exec(QString() + "UPDATE `" + TABLE_SYSTEM + "` SET `value` = '" + last_wallet_id + "' WHERE `key` = '" + SYSTEM_LAST_WALLET_ID + "'");

        if (query.lastError().type() == QSqlError::ConnectionError) {
            throw "msg.general.nointernet";
        } else if (query.lastError().type() == QSqlError::StatementError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.statementerror";
        } else if (query.lastError().type() == QSqlError::UnknownError) {
            cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
            cerr << query.lastError().text().toStdString() << endl;
            throw "msg.general.unknownerror";
        }

        return last_wallet_id;
    }
}

void SqlManager::registerUser(QString login, QString password, QString firstName, QString secondName, QString middleName) {
    QSqlQuery query(db);
    query.exec(QString() + "INSERT INTO `" + TABLE_USERS + "` (`" + USER_PHONE + "`, `" + USER_PASSWORD + "`, `" + USER_FIRST_NAME + "`, `" + USER_SECOND_NAME + "`, `" + USER_MIDDLE_NAME + "`, `" + USER_REG_DATE + "`, `" + USER_BALANCE + "`, `" + USER_WALLET_ID + "`) VALUES ('" + login + "', '" + QString::fromStdString(md5(md5(password.toStdString()))) + "', '" + firstName + "', '" + secondName + "', '" + middleName + "', '" + QString::number(getCurrentTimestamp()) + "', '0', '" + updateLastWalletId() + "')");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }
}

void SqlManager::changePassword(QString login, QString password) {
    QSqlQuery query(db);
    if (login.startsWith("admin_")) {
        query.exec(QString() + "UPDATE `" + TABLE_ADMINS + "` SET `" + USER_PASSWORD + "` = '" + password + "' WHERE `" + ADMIN_LOGIN + "` = '" + login + "'");
    } else {
        query.exec(QString() + "UPDATE `" + TABLE_USERS + "` SET `" + USER_PASSWORD + "` = '" + password + "' WHERE `" + USER_PHONE + "` = '" + login + "'");
    }

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }
}

void SqlManager::sendMoney(double amount, QString fromWallet, QString toWallet) {
    QSqlQuery query(db);
    query.exec(QString() + "UPDATE `" + TABLE_USERS + "` SET `" + USER_BALANCE + "` = `" + USER_BALANCE + "` + CASE `" + USER_WALLET_ID + "` WHEN '" + toWallet + "' THEN " + QString::number(amount) + " WHEN '" + fromWallet + "' THEN -" + QString::number(amount) + " END WHERE `" + USER_WALLET_ID + "` IN (" + toWallet + ", " + fromWallet + ")");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }
}

map<int, DefaultUser> SqlManager::loadUsers() {
    map<int, DefaultUser> users;

    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_USERS + "`");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    while (query.next()) {
        int id = query.value(USER_ID).toInt();
        QString login = query.value(USER_PHONE).toString().toUtf8().constData();
        QString encryptedPassword = query.value(USER_PASSWORD).toString().toUtf8().constData();
        QString firstName = query.value(USER_FIRST_NAME).toString().toUtf8().constData();
        QString secondName = query.value(USER_SECOND_NAME).toString().toUtf8().constData();
        QString middleName = query.value(USER_MIDDLE_NAME).toString().toUtf8().constData();
        long regDate = query.value(USER_REG_DATE).toLongLong();
        double balance = query.value(USER_BALANCE).toDouble();
        QString walletId = query.value(USER_WALLET_ID).toString().toUtf8().constData();

        users.insert(pair<int, DefaultUser>(id, DefaultUser(id, login, encryptedPassword, firstName, secondName, middleName, regDate, balance, walletId)));
    }

    return users;
}

map<int, AdminUser> SqlManager::loadAdmins() {
    map<int, AdminUser> admins;

    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_ADMINS + "`");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    while (query.next()) {
        int id = query.value(ADMIN_ID).toInt();
        QString login = query.value(ADMIN_LOGIN).toString().toUtf8().constData();
        QString encryptedPassword = query.value(ADMIN_PASSWORD).toString().toUtf8().constData();
        QString firstName = query.value(ADMIN_FIRST_NAME).toString().toUtf8().constData();
        QString secondName = query.value(ADMIN_SECOND_NAME).toString().toUtf8().constData();
        QString middleName = query.value(ADMIN_MIDDLE_NAME).toString().toUtf8().constData();
        bool addAdmin = query.value(ADMIN_CAN_ADD_ADMIN).toBool();
        bool editPhone = query.value(ADMIN_CAN_EDIT_PHONE).toBool();
        bool editPassword = query.value(ADMIN_CAN_EDIT_PASSWORD).toBool();
        bool editPersonalData = query.value(ADMIN_CAN_EDIT_PERSONAL_DATA).toBool();
        bool editBalance = query.value(ADMIN_CAN_EDIT_BALANCE).toBool();

        admins.insert(pair<int, AdminUser>(id, AdminUser(id, login, encryptedPassword, firstName, secondName, middleName, addAdmin, editPhone, editPassword, editPersonalData, editBalance)));
    }

    return admins;
}

void SqlManager::editUser(DefaultUser editableUser, int id, QString phone, QString password, double balance, QString firstName, QString secondName, QString middleName) {
    QString request = QString() + "UPDATE `" + TABLE_USERS + "` SET ";
    if (editableUser.getLogin() != phone) {
        request.append(QString() + "`" + USER_PHONE + "` = '" + phone + "', ");
    }
    if (!password.isEmpty()) {
        request.append(QString() + "`" + USER_PASSWORD + "` = '" + QString::fromStdString(md5(md5(password.toStdString()))) + "', ");
    }
    if (editableUser.getBalance() != balance) {
        request.append(QString() + "`" + USER_BALANCE + "` = '" + QString::number(balance) + "', ");
    }
    if (editableUser.getFirstName() != firstName) {
        request.append(QString() + "`" + USER_FIRST_NAME + "` = '" + firstName + "', ");
    }
    if (editableUser.getSecondName() != secondName) {
        request.append(QString() + "`" + USER_SECOND_NAME + "` = '" + secondName + "', ");
    }
    if (editableUser.getMiddleName() != middleName) {
        request.append(QString() + "`" + USER_MIDDLE_NAME + "` = '" + middleName + "', ");
    }

    if (!request.endsWith("', ")) {
        return;
    }

    request.truncate(request.length() - 2);
    request.append(QString() + " WHERE `" + USER_ID + "` = '" + QString::number(id) + "'");

    QSqlQuery query(db);
    query.exec(request);

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }
}

bool SqlManager::isAdminExists(QString login) {
    QSqlQuery query(db);
    query.exec(QString() + "SELECT * FROM `" + TABLE_ADMINS + "` WHERE `" + ADMIN_LOGIN + "` = '" + login + "'");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }

    if (query.size() == 0) {
        return false;
    }

    return true;
}

void SqlManager::registerAdmin(QString login, QString password, QString firstName, QString secondName, QString middleName, bool addAdmin, bool editPhone, bool editPassword, bool editPersonalData, bool editBalance) {
    QSqlQuery query(db);
    query.exec(QString() + "INSERT INTO `" + TABLE_ADMINS + "` (`" + ADMIN_LOGIN + "`, `" + ADMIN_PASSWORD "`, `" + ADMIN_FIRST_NAME "`, `" + ADMIN_SECOND_NAME "`, `" + ADMIN_MIDDLE_NAME + "`, `" + ADMIN_CAN_ADD_ADMIN "`, `" + ADMIN_CAN_EDIT_PHONE + "`, `" + ADMIN_CAN_EDIT_PASSWORD + "`, `" + ADMIN_CAN_EDIT_PERSONAL_DATA + "`, `" + ADMIN_CAN_EDIT_BALANCE + "`) VALUES ('" + login + "', '" + QString::fromStdString(md5(md5(password.toStdString()))) + "', '" + firstName + "', '" + secondName + "', '" + middleName + "', '" + QString::number(addAdmin) + "', '" + QString::number(editPhone) + "', '" + QString::number(editPassword) + "', '" + QString::number(editPersonalData) + "', '" + QString::number(editBalance) + "')");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }
}

void SqlManager::deleteAdmin(QString login) {
    QSqlQuery query(db);
    query.exec(QString() + "DELETE FROM `" + TABLE_ADMINS + "` WHERE `" + ADMIN_LOGIN + "` = '" + login + "'");

    if (query.lastError().type() == QSqlError::ConnectionError) {
        throw "msg.general.nointernet";
    } else if (query.lastError().type() == QSqlError::StatementError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.statementerror";
    } else if (query.lastError().type() == QSqlError::UnknownError) {
        cerr << "Method \"" << __FUNCTION__ << "\" caught an exception!" << endl;
        cerr << query.lastError().text().toStdString() << endl;
        throw "msg.general.unknownerror";
    }
}
