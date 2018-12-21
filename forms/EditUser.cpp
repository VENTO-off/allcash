#include "ui_EditUser.h"

#include "forms/EditUser.h"

#include "classes/User.h"

#include "managers/NotifyManager.h"
#include "managers/SqlManager.h"
#include "managers/LocaleManager.h"

#include "utils/StringUtils.h"

using StringUtils::isPhoneValid;
using StringUtils::isAmountValid;
using StringUtils::isPasswordValid;
using StringUtils::isPersonalNameValid;
using StringUtils::formatAmount;

extern User * loggedAccount;
extern SqlManager sqlManager;
extern LocaleManager localeManager;

EditUser::EditUser(AdminClient *adminForm, DefaultUser editableUser, int id, QWidget *parent) : QDialog(parent), editUserForm(new Ui::EditUser) {
    editUserForm->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->adminForm = adminForm;
    this->editableUser = editableUser;
    this->id = id;

    applyLocale();
    loadData();
}

EditUser::~EditUser() {
    delete editUserForm;
}

void EditUser::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    editUserForm->title->setText(localeManager.translate(editUserForm->title->text()));
    editUserForm->phoneLabel->setText(localeManager.translate(editUserForm->phoneLabel->text()));
    editUserForm->newPasswordLabel->setText(localeManager.translate(editUserForm->newPasswordLabel->text()));
    editUserForm->balanceLabel->setText(localeManager.translate(editUserForm->balanceLabel->text()));
    editUserForm->firstNameLabel->setText(localeManager.translate(editUserForm->firstNameLabel->text()));
    editUserForm->secondNameLabel->setText(localeManager.translate(editUserForm->secondNameLabel->text()));
    editUserForm->middleNameLabel->setText(localeManager.translate(editUserForm->middleNameLabel->text()));
    editUserForm->confirmButton->setText(localeManager.translate(editUserForm->confirmButton->text()));
    editUserForm->cancelButton->setText(localeManager.translate(editUserForm->cancelButton->text()));
}

void EditUser::loadData() {
    //Phone
    editUserForm->phoneField->setText(editableUser.getLogin());
    if (!static_cast<AdminUser*>(loggedAccount)->canEditPhone()) {
        editUserForm->phoneField->setEnabled(false);
        editUserForm->phoneLabel->setEnabled(false);
    }

    //Password
    if (!static_cast<AdminUser*>(loggedAccount)->canEditPassword()) {
        editUserForm->newPasswordField->setEnabled(false);
        editUserForm->newPasswordLabel->setEnabled(false);
    }

    //Balance
    editUserForm->balanceField->setText(formatAmount(editableUser.getBalance()));
    if (!static_cast<AdminUser*>(loggedAccount)->canEditBalance()) {
        editUserForm->balanceField->setEnabled(false);
        editUserForm->balanceLabel->setEnabled(false);
    }

    //Personal data
    editUserForm->firstNameField->setText(editableUser.getFirstName());
    editUserForm->secondNameField->setText(editableUser.getSecondName());
    editUserForm->middleNameField->setText(editableUser.getMiddleName());
    if (!static_cast<AdminUser*>(loggedAccount)->canEditPersonalData()) {
        editUserForm->firstNameField->setEnabled(false);
        editUserForm->firstNameLabel->setEnabled(false);
        editUserForm->secondNameField->setEnabled(false);
        editUserForm->secondNameLabel->setEnabled(false);
        editUserForm->middleNameField->setEnabled(false);
        editUserForm->middleNameLabel->setEnabled(false);
    }
}

void EditUser::on_confirmButton_clicked() {
    //Check phone string
    QString phoneValue = editUserForm->phoneField->text();
    if (!isPhoneValid(phoneValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidphone"));
        return;
    }

    //Check phone duplicates
    if (editableUser.getLogin() != phoneValue) {
        try {
            if (sqlManager.isUserExists(phoneValue)) {
                NotifyManager notify(ERROR);
                notify.showNotify(localeManager.translate("msg.general.phoneregistered").replace("%phone%", phoneValue));
                return;
            }
        } catch (const char* exception) {
            NotifyManager notify(CRITICAL);
            notify.showNotify(localeManager.translate(exception));
        }
    }

    //Check password string
    QString passwordValue = editUserForm->newPasswordField->text();
    if (!passwordValue.isEmpty() && !isPasswordValid(passwordValue)) {
        NotifyManager notify(WARN);
        notify.showNotify(localeManager.translate("msg.general.weakpassword"));
        return;
    }

    //Check money string
    if (!isAmountValid(editUserForm->balanceField->text())) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.money.invalidformat"));
        return;
    }
    double balance = fabs(editUserForm->balanceField->text().toDouble());

    //Check first name
    QString firstNameValue = editUserForm->firstNameField->text();
    if (!isPersonalNameValid(firstNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidfirstname"));
        return;
    }

    //Check second name
    QString secondNameValue = editUserForm->secondNameField->text();
    if (!isPersonalNameValid(secondNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidsecondname"));
        return;
    }

    //Check middle name
    QString middleNameValue = editUserForm->middleNameField->text();
    if (!isPersonalNameValid(middleNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidmiddlename"));
        return;
    }

    try {
        sqlManager.editUser(editableUser,
                            id,
                            phoneValue,
                            passwordValue,
                            balance,
                            firstNameValue,
                            secondNameValue,
                            middleNameValue);
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    this->close();

    NotifyManager notify;
    notify.showNotify(localeManager.translate("msg.admin.usermodified"));

    adminForm->updateData();
}

void EditUser::on_cancelButton_clicked() {
    this->close();
}
