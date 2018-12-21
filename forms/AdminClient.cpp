#include "ui_AdminClient.h"

#include "forms/AdminClient.h"
#include "forms/ChangePassword.h"
#include "forms/EditUser.h"
#include "forms/AddAdmin.h"

#include "classes/User.h"

#include "managers/SqlManager.h"
#include "managers/NotifyManager.h"
#include "managers/LocaleManager.h"
#include "managers/WindowManager.h"

#include "utils/StringUtils.h"
#include "utils/TimeUtils.h"

using TimeUtils::getDate;
using StringUtils::formatAmount;

extern User * loggedAccount;
extern SqlManager sqlManager;
extern LocaleManager localeManager;

AdminClient::AdminClient(QWidget *parent) : QWidget(parent), adminForm(new Ui::AdminClient) {
    adminForm->setupUi(this);
    this->setFixedSize(this->size());
    adminForm->usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    adminForm->adminsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    applyLocale();
    initIcons();
    updateData();
}

AdminClient::~AdminClient() {
    delete adminForm;
}

void AdminClient::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    adminForm->welcomeLabel->setText(localeManager.translate(adminForm->welcomeLabel->text()).replace("%name%", loggedAccount->getFirstName() + " " + loggedAccount->getSecondName()));
    adminForm->accountGroupBox->setTitle(localeManager.translate(adminForm->accountGroupBox->title()));
    adminForm->registeredUsersLabel->setText(localeManager.translate(adminForm->registeredUsersLabel->text()));
    adminForm->registeredAdminsLabel->setText(localeManager.translate(adminForm->registeredAdminsLabel->text()));
    adminForm->addAdminButton->setText(localeManager.translate(adminForm->addAdminButton->text()));
    adminForm->changePasswordButton->setText(localeManager.translate(adminForm->changePasswordButton->text()));
    adminForm->logOutButton->setText(localeManager.translate(adminForm->logOutButton->text()));
    for (int i = 0; i < adminForm->manageUsersTabs->count(); i++) {
        adminForm->manageUsersTabs->setTabText(i, localeManager.translate(adminForm->manageUsersTabs->tabText(i)));
    }
    adminForm->findUserLabel->setText(localeManager.translate(adminForm->findUserLabel->text()));
    adminForm->searchUsersByLabel->setText(localeManager.translate(adminForm->searchUsersByLabel->text()));
    for (int i = 0; i < adminForm->usersTable->columnCount(); i++) {
        adminForm->usersTable->horizontalHeaderItem(i)->setText(localeManager.translate(adminForm->usersTable->horizontalHeaderItem(i)->text()));
    }
    adminForm->findAdminLabel->setText(localeManager.translate(adminForm->findAdminLabel->text()));
    adminForm->searchAdminsByLabel->setText(localeManager.translate(adminForm->searchAdminsByLabel->text()));
    for (int i = 0; i < adminForm->adminsTable->columnCount(); i++) {
        adminForm->adminsTable->horizontalHeaderItem(i)->setText(localeManager.translate(adminForm->adminsTable->horizontalHeaderItem(i)->text()));
    }
    adminForm->helpButton->setIcon(QIcon(":/res/assets/images/help.png"));
    adminForm->helpButton->setIconSize(QSize(16, 16));
    localeManager.showLocales(adminForm->localeChanger);
}

void AdminClient::initIcons() {
    adminForm->addAdminButton->setIcon(QIcon(":/res/assets/images/admin.png"));
    adminForm->addAdminButton->setIconSize(QSize(16, 16));
    adminForm->changePasswordButton->setIcon(QIcon(":/res/assets/images/password.png"));
    adminForm->changePasswordButton->setIconSize(QSize(16, 16));
    adminForm->logOutButton->setIcon(QIcon(":/res/assets/images/logout.png"));
    adminForm->logOutButton->setIconSize(QSize(16, 16));
}

void AdminClient::updateData() {
    try {
        this->users = sqlManager.loadUsers();
        this->admins = sqlManager.loadAdmins();
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    adminForm->registeredUsersValue->setText(QString::number(users.size()));
    adminForm->registeredAdminsValue->setText(QString::number(admins.size()));

    updateUsers(this->users);
    updateAdmins(this->admins);

    adminForm->findUserField->setText(0);
    adminForm->findAdminField->setText(0);
}

void AdminClient::onEditUserClicked() {
    QPushButton* editButton = qobject_cast<QPushButton*>(sender());
    if (!editButton) {
        return;
    }

    QVariant var = editButton->property("id");
    if (!var.isValid()) {
        return;
    }

    map<int, DefaultUser>::iterator it;
    it = users.find(var.toInt());
    if (it != users.end()) {
        EditUser *editUser = new EditUser(this, it->second, it->first);
        editUser->setModal(true);
        editUser->show();
    }
}

void AdminClient::onDeleteAdminClicked() {
    QPushButton* editButton = qobject_cast<QPushButton*>(sender());
    if (!editButton) {
        return;
    }

    QVariant var = editButton->property("id");
    if (!var.isValid()) {
        return;
    }

    if (!static_cast<AdminUser*>(loggedAccount)->canAddAdmin()) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.admin.nopermissionaddadmin"));
        return;
    }

    map<int, AdminUser>::iterator it;
    it = admins.find(var.toInt());
    if (it != admins.end()) {
        AdminUser admin = it->second;
        if (admin.getLogin() == loggedAccount->getLogin()) {
            NotifyManager notify(WARN);
            notify.showNotify(localeManager.translate("msg.admin.cantdeleteyourself"));
            return;
        }

        QMessageBox::StandardButton confirmBox;
        confirmBox = QMessageBox::question(this,
                                      "AllCash :: " + localeManager.translate("msg.general.confirmaction"),
                                      localeManager.translate("msg.admin.suretodeleteadmin") + " \"" + admin.getLogin() + "\"?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (confirmBox == QMessageBox::Yes) {
            try {
                sqlManager.deleteAdmin(admin.getLogin());
            } catch (const char* exception) {
                NotifyManager notify(CRITICAL);
                notify.showNotify(localeManager.translate(exception));
            }
            updateData();
        }
    }
}

void AdminClient::updateUsers(map<int, DefaultUser> users) {
    adminForm->usersTable->setRowCount(users.size());

    map<int, DefaultUser>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        int row = distance(users.begin(), it);
        int id = it->first;
        DefaultUser user = it->second;

        QPushButton* editButton = new QPushButton(localeManager.translate("gui.adminclient.button.edituser"));
        editButton->setProperty("id", id);
        connect(editButton, SIGNAL(clicked()), SLOT(onEditUserClicked()));

        adminForm->usersTable->setCellWidget(row, 0, editButton);
        adminForm->usersTable->setItem(row, 1, new QTableWidgetItem(QString::number(it->first)));
        adminForm->usersTable->setItem(row, 2, new QTableWidgetItem(user.getLogin()));
        adminForm->usersTable->setItem(row, 3, new QTableWidgetItem(user.getFirstName()));
        adminForm->usersTable->setItem(row, 4, new QTableWidgetItem(user.getSecondName()));
        adminForm->usersTable->setItem(row, 5, new QTableWidgetItem(user.getMiddleName()));
        adminForm->usersTable->setItem(row, 6, new QTableWidgetItem(getDate(user.getRegDate())));
        adminForm->usersTable->setItem(row, 7, new QTableWidgetItem(formatAmount(user.getBalance())));
        adminForm->usersTable->setItem(row, 8, new QTableWidgetItem(user.getWalletId()));
    }
}

void AdminClient::updateAdmins(map<int, AdminUser> admins) {
    adminForm->adminsTable->setRowCount(admins.size());

    map<int, AdminUser>::iterator it;
    for (it = admins.begin(); it != admins.end(); it++) {
        int row = distance(admins.begin(), it);
        int id = it->first;
        AdminUser admin = it->second;

        QPushButton* deleteButton = new QPushButton(localeManager.translate("gui.adminclient.button.deleteadmin"));
        deleteButton->setProperty("id", id);
        connect(deleteButton, SIGNAL(clicked()), SLOT(onDeleteAdminClicked()));

        adminForm->adminsTable->setCellWidget(row, 0, deleteButton);
        adminForm->adminsTable->setItem(row, 1, new QTableWidgetItem(QString::number(it->first)));
        adminForm->adminsTable->setItem(row, 2, new QTableWidgetItem(admin.getLogin()));
        adminForm->adminsTable->setItem(row, 3, new QTableWidgetItem(admin.getFirstName()));
        adminForm->adminsTable->setItem(row, 4, new QTableWidgetItem(admin.getSecondName()));
        adminForm->adminsTable->setItem(row, 5, new QTableWidgetItem(admin.getMiddleName()));
        adminForm->adminsTable->setItem(row, 6, new QTableWidgetItem(admin.canAddAdmin() ? QChar(0x2714) : QChar(0x2718)));
        adminForm->adminsTable->setItem(row, 7, new QTableWidgetItem(admin.canEditPhone() ? QChar(0x2714) : QChar(0x2718)));
        adminForm->adminsTable->setItem(row, 8, new QTableWidgetItem(admin.canEditPassword() ? QChar(0x2714) : QChar(0x2718)));
        adminForm->adminsTable->setItem(row, 9, new QTableWidgetItem(admin.canEditPersonalData() ? QChar(0x2714) : QChar(0x2718)));
        adminForm->adminsTable->setItem(row, 10, new QTableWidgetItem(admin.canEditBalance() ? QChar(0x2714) : QChar(0x2718)));
    }
}

void AdminClient::on_changePasswordButton_clicked() {
    ChangePassword *changePassword = new ChangePassword();
    changePassword->setModal(true);
    changePassword->show();
}

void AdminClient::on_logOutButton_clicked() {
    this->close();
    loggedAccount->logOut();
    loggedAccount = 0;
}

void AdminClient::on_findUserField_textEdited(const QString &arg1) {
    QString findValue = adminForm->findUserField->text();
    if (findValue.isEmpty()) {
        updateUsers(this->users);
        return;
    }

    map<int, DefaultUser> filteredUsers;

    map<int, DefaultUser>::iterator it;
    for (it = users.begin(); it != users.end(); it++) {
        int id = it->first;
        DefaultUser user = it->second;

        if (user.getLogin().contains(findValue) ||
                user.getSecondName().contains(findValue) ||
                user.getWalletId().contains(findValue)) {
            filteredUsers.insert(pair<int, DefaultUser>(id, user));
        }
    }

    updateUsers(filteredUsers);
}

void AdminClient::on_findAdminField_textEdited(const QString &arg1) {
    QString findValue = adminForm->findAdminField->text();
    if (findValue.isEmpty()) {
        updateAdmins(this->admins);
        return;
    }

    map<int, AdminUser> filteredAdmins;

    map<int, AdminUser>::iterator it;
    for (it = admins.begin(); it != admins.end(); it++) {
        int id = it->first;
        AdminUser admin = it->second;

        if (admin.getLogin().contains(findValue) ||
                admin.getSecondName().contains(findValue)) {
            filteredAdmins.insert(pair<int, AdminUser>(id, admin));
        }
    }

    updateAdmins(filteredAdmins);
}

void AdminClient::on_addAdminButton_clicked() {
    if (!static_cast<AdminUser*>(loggedAccount)->canAddAdmin()) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.admin.nopermissionaddadmin"));
        return;
    }

    AddAdmin *addAdmin = new AddAdmin(this);
    addAdmin->setModal(true);
    addAdmin->show();
}

void AdminClient::on_helpButton_clicked() {
    WindowManager(localeManager.translate("form.title.help"), localeManager.translate("help.adminclient"));
}
