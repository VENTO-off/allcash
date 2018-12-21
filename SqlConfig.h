#ifndef SQLCONFIG_H
#define SQLCONFIG_H

//Version
#define VERSION "1.0"

//MySql Database
#define DB_HOST "185.17.3.218"
#define DB_USER "root"
#define DB_NAME "allcash"
#define DB_PASS "dGV4dGVzdA=="      //encoded pass

//Tables
#define TABLE_USERS "allcash_users"
#define TABLE_ADMINS "allcash_admins"
#define TABLE_SYSTEM "allcash_system"

//Columns
#define USER_ID "id"
#define USER_PHONE "phone"
#define USER_PASSWORD "password"
#define USER_FIRST_NAME "first_name"
#define USER_SECOND_NAME "second_name"
#define USER_MIDDLE_NAME "middle_name"
#define USER_REG_DATE "reg_date"
#define USER_BALANCE "balance"
#define USER_WALLET_ID "wallet_id"

#define ADMIN_ID "id"
#define ADMIN_LOGIN "login"
#define ADMIN_PASSWORD "password"
#define ADMIN_FIRST_NAME "first_name"
#define ADMIN_SECOND_NAME "second_name"
#define ADMIN_MIDDLE_NAME "middle_name"
#define ADMIN_CAN_ADD_ADMIN "can_add_admin"
#define ADMIN_CAN_EDIT_PHONE "can_edit_phone"
#define ADMIN_CAN_EDIT_PASSWORD "can_edit_password"
#define ADMIN_CAN_EDIT_PERSONAL_DATA "can_edit_personal_data"
#define ADMIN_CAN_EDIT_BALANCE "can_edit_balance"

#define SYSTEM_LAST_WALLET_ID "last_wallet_id"
#define SYSTEM_LAST_VERSION "last_version"

#endif
