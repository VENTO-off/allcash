#include "ui_AddAdmin.h"

#include "forms/AddAdmin.h"

#include "managers/SqlManager.h"
#include "managers/NotifyManager.h"
#include "managers/LocaleManager.h"

#include "utils/StringUtils.h"

using StringUtils::isAdminLoginValid;
using StringUtils::isPasswordValid;
using StringUtils::isPersonalNameValid;

extern SqlManager sqlManager;
extern LocaleManager localeManager;

AddAdmin::AddAdmin(AdminClient *adminForm, QWidget *parent) : QDialog(parent), addAdminForm(new Ui::AddAdmin) {
    addAdminForm->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->adminForm = adminForm;

    applyLocale();
}

AddAdmin::~AddAdmin() {
    delete addAdminForm;
}

void AddAdmin::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    addAdminForm->title->setText(localeManager.translate(addAdminForm->title->text()));
    addAdminForm->loginLabel->setText(localeManager.translate(addAdminForm->loginLabel->text()));
    addAdminForm->passwordLabel->setText(localeManager.translate(addAdminForm->passwordLabel->text()));
    addAdminForm->password2Label->setText(localeManager.translate(addAdminForm->password2Label->text()));
    addAdminForm->firstNameLabel->setText(localeManager.translate(addAdminForm->firstNameLabel->text()));
    addAdminForm->secondNameLabel->setText(localeManager.translate(addAdminForm->secondNameLabel->text()));
    addAdminForm->middleNameLabel->setText(localeManager.translate(addAdminForm->middleNameLabel->text()));
    addAdminForm->permissionsBox->setTitle(localeManager.translate(addAdminForm->permissionsBox->title()));
    addAdminForm->addAdminCheckBox->setText(localeManager.translate(addAdminForm->addAdminCheckBox->text()));
    addAdminForm->editPhoneCheckBox->setText(localeManager.translate(addAdminForm->editPhoneCheckBox->text()));
    addAdminForm->editPasswordCheckBox->setText(localeManager.translate(addAdminForm->editPasswordCheckBox->text()));
    addAdminForm->editPersonalDataCheckBox->setText(localeManager.translate(addAdminForm->editPersonalDataCheckBox->text()));
    addAdminForm->editBalanceCheckBox->setText(localeManager.translate(addAdminForm->editBalanceCheckBox->text()));
    addAdminForm->confirmButton->setText(localeManager.translate(addAdminForm->confirmButton->text()));
    addAdminForm->cancelButton->setText(localeManager.translate(addAdminForm->cancelButton->text()));
}

void AddAdmin::on_loginField_textEdited(const QString &arg1) {
    QString loginValue = arg1;

    QString prefix = "admin_";
    if (loginValue.length() < prefix.size()) {
        loginValue = prefix;
    } else {
        loginValue.replace(0, prefix.size(), prefix);
    }

    //Check for allowed symbols
    for (int i = prefix.size(); i < loginValue.length(); i++) {
        if (!loginValue.at(i).isDigit() && !loginValue.at(i).isLetter()) {
            loginValue.remove(i, 1);
        }
    }

    if (addAdminForm->loginField->text() != loginValue) {
        addAdminForm->loginField->setText(loginValue);
    }
}

void AddAdmin::on_confirmButton_clicked() {
    //Check login
    QString loginValue = addAdminForm->loginField->text();
    if (!isAdminLoginValid(loginValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.admin.invalidlogin"));
        return;
    }

    //Check login duplicates
    try {
        if (sqlManager.isAdminExists(loginValue)) {
            NotifyManager notify(ERROR);
            notify.showNotify(localeManager.translate("msg.admin.logintaken"));
            return;
        }
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    //Check password
    QString passwordValue = addAdminForm->passwordField->text();
    if (!isPasswordValid(passwordValue)) {
        NotifyManager notify(WARN);
        notify.showNotify(localeManager.translate("msg.general.weakpassword"));
        return;
    }

    //Check passwords match
    QString password2Value = addAdminForm->password2Field->text();
    if (passwordValue != password2Value) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.passwordnotmatch"));
        return;
    }

    //Check first name
    QString firstNameValue = addAdminForm->firstNameField->text();
    if (!isPersonalNameValid(firstNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidfirstname"));
        return;
    }

    //Check second name
    QString secondNameValue = addAdminForm->secondNameField->text();
    if (!isPersonalNameValid(secondNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidsecondname"));
        return;
    }

    //Check middle name
    QString middleNameValue = addAdminForm->middleNameField->text();
    if (!isPersonalNameValid(middleNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidmiddlename"));
        return;
    }

    try {
        sqlManager.registerAdmin(loginValue,
                                 passwordValue,
                                 firstNameValue,
                                 secondNameValue,
                                 middleNameValue,
                                 addAdminForm->addAdminCheckBox->isChecked(),
                                 addAdminForm->editPhoneCheckBox->isChecked(),
                                 addAdminForm->editPasswordCheckBox->isChecked(),
                                 addAdminForm->editPersonalDataCheckBox->isChecked(),
                                 addAdminForm->editBalanceCheckBox->isChecked());
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }


    this->close();

    NotifyManager notify;
    notify.showNotify(localeManager.translate("msg.admin.adminregistered"));

    adminForm->updateData();
}

void AddAdmin::on_cancelButton_clicked() {
    this->close();
}
