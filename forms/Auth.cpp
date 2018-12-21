#include "ui_Auth.h"

#include "forms/Auth.h"
#include "forms/Register.h"
#include "forms/UserClient.h"

#include "classes/User.h"
#include "classes/ClickableQLabel.h"

#include "managers/NotifyManager.h"
#include "managers/SqlManager.h"
#include "managers/LocaleManager.h"
#include "managers/SettingsManager.h"
#include "managers/WindowManager.h"

extern User * loggedAccount;
extern SqlManager sqlManager;
extern LocaleManager localeManager;
extern SettingsManager settingsManager;

Auth::Auth(QString login, QWidget *parent) : QMainWindow(parent), authForm(new Ui::Auth) {
    authForm->setupUi(this);
    this->setFixedSize(this->size());
    authForm->versionLabel->setText(authForm->versionLabel->text().replace("%version%", ("v" + QString(VERSION))));
    authForm->loginField->setText(settingsManager.getLastAccount());
    if (!login.isEmpty()) {
        authForm->loginField->setText(login);
    }
    connect(authForm->regLink, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));

    applyLocale();
    initIcons();
}

Auth::~Auth() {
    delete authForm;
}

void Auth::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    authForm->title->setText(localeManager.translate(authForm->title->text()));
    authForm->phoneLabel->setText(localeManager.translate(authForm->phoneLabel->text()));
    authForm->passwordLabel->setText(localeManager.translate(authForm->passwordLabel->text()));
    authForm->authButton->setText(localeManager.translate(authForm->authButton->text()));
    authForm->infoRegLink->setText(localeManager.translate(authForm->infoRegLink->text()));
    authForm->regLink->setText(localeManager.translate(authForm->regLink->text()));
    authForm->developersButton->setText((localeManager.translate(authForm->developersButton->text())));
    localeManager.showLocales(authForm->localeChanger);
}

void Auth::initIcons() {
    authForm->helpButton->setIcon(QIcon(":/res/assets/images/help.png"));
    authForm->helpButton->setIconSize(QSize(16, 16));
}

void Auth::checkFields() {
    if (!authForm->loginField->text().isEmpty() && !authForm->passwordField->text().isEmpty()) {
        authForm->authButton->setEnabled(true);
    } else {
        authForm->authButton->setEnabled(false);
    }
}

void Auth::Mouse_Pressed() {
    this->close();
    Register *registerForm = new Register();
    registerForm->show();
}

void Auth::on_loginField_textEdited(const QString &arg1) {
    checkFields();
}

void Auth::on_passwordField_textEdited(const QString &arg1) {
    checkFields();
}

void Auth::on_authButton_clicked() {
    QString login = authForm->loginField->text();
    QString password = authForm->passwordField->text();

    try {
        loggedAccount = sqlManager.authUser(login, password);
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    if (loggedAccount == 0) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.user.invalidphoneorpassword"));
        return;
    }

    this->close();
    loggedAccount->logIn();
}

void Auth::on_helpButton_clicked() {
    WindowManager(localeManager.translate("form.title.help"), localeManager.translate("help.auth"));
}

void Auth::on_developersButton_clicked() {
    WindowManager(localeManager.translate("form.title.developers"), localeManager.translate("developers.list"));
}
