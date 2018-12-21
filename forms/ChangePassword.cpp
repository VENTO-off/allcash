#include "ui_ChangePassword.h"

#include "forms/ChangePassword.h"

#include "classes/User.h"

#include "managers/NotifyManager.h"
#include "managers/SqlManager.h"
#include "managers/LocaleManager.h"

#include "utils/StringUtils.h"

#include "libs/md5/md5.h"

using StringUtils::isPasswordValid;

extern User * loggedAccount;
extern SqlManager sqlManager;
extern LocaleManager localeManager;

ChangePassword::ChangePassword(QWidget *parent) : QDialog(parent), changePasswordForm(new Ui::ChangePassword) {
    changePasswordForm->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    applyLocale();
}

ChangePassword::~ChangePassword() {
    delete changePasswordForm;
}

void ChangePassword::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    changePasswordForm->title->setText(localeManager.translate(changePasswordForm->title->text()));
    changePasswordForm->oldPasswordLabel->setText(localeManager.translate(changePasswordForm->oldPasswordLabel->text()));
    changePasswordForm->newPasswordLabel->setText(localeManager.translate(changePasswordForm->newPasswordLabel->text()));
    changePasswordForm->newPassword2Label->setText(localeManager.translate(changePasswordForm->newPassword2Label->text()));
    changePasswordForm->changePasswordButton->setText(localeManager.translate(changePasswordForm->changePasswordButton->text()));
}

void ChangePassword::checkFields() {
    if (!changePasswordForm->oldPasswordField->text().isEmpty() &&
            !changePasswordForm->newPasswordField->text().isEmpty() &&
            !changePasswordForm->newPassword2Field->text().isEmpty()) {
        changePasswordForm->changePasswordButton->setEnabled(true);
    } else {
        changePasswordForm->changePasswordButton->setEnabled(false);
    }
}

void ChangePassword::on_oldPasswordField_textEdited(const QString &arg1) {
    checkFields();
}

void ChangePassword::on_newPasswordField_textEdited(const QString &arg1) {
    checkFields();
}

void ChangePassword::on_newPassword2Field_textEdited(const QString &arg1) {
    checkFields();
}

void ChangePassword::on_changePasswordButton_clicked() {
    QString oldPassword = changePasswordForm->oldPasswordField->text();
    if (md5(md5(oldPassword.toStdString())) != loggedAccount->getEncryptedPassword().toStdString()) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidoldpassword"));
        return;
    }

    QString newPassword = changePasswordForm->newPasswordField->text();
    QString newPassword2 = changePasswordForm->newPassword2Field->text();
    if (newPassword != newPassword2) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.passwordnotmatch"));
        return;
    }

    if (oldPassword == newPassword) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.passwordmustnotmatchwithprevious"));
        return;
    }

    if (!isPasswordValid(newPassword)) {
        NotifyManager notify(WARN);
        notify.showNotify(localeManager.translate("msg.general.weakpassword"));
        return;
    }

    try {
        sqlManager.changePassword(loggedAccount->getLogin(), QString::fromStdString(md5(md5(newPassword.toStdString()))));
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    loggedAccount->changeEncryptedPassword(QString::fromStdString(md5(md5(newPassword.toStdString()))));

    this->close();

    NotifyManager notify;
    notify.showNotify(localeManager.translate("msg.general.passwordchanged"));
}
