#include "ui_Register.h"

#include "forms/Register.h"
#include "forms/Auth.h"

#include "managers/NotifyManager.h"
#include "managers/SqlManager.h"
#include "managers/LocaleManager.h"
#include "managers/WindowManager.h"

#include "utils/StringUtils.h"

using StringUtils::isPhoneValid;
using StringUtils::isPasswordValid;
using StringUtils::isPersonalNameValid;

extern SqlManager sqlManager;
extern LocaleManager localeManager;

Register::Register(QWidget *parent) : QWidget(parent), registerForm(new Ui::Register) {
    registerForm->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(registerForm->loginLink, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));

    applyLocale();
    initIcons();
}

Register::~Register() {
    delete registerForm;
}

void Register::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    registerForm->title->setText(localeManager.translate(registerForm->title->text()));
    registerForm->phoneLabel->setText(localeManager.translate(registerForm->phoneLabel->text()));
    registerForm->passwordLabel->setText(localeManager.translate(registerForm->passwordLabel->text()));
    registerForm->password2Label->setText(localeManager.translate(registerForm->password2Label->text()));
    registerForm->firstNameLabel->setText(localeManager.translate(registerForm->firstNameLabel->text()));
    registerForm->secondNameLabel->setText(localeManager.translate(registerForm->secondNameLabel->text()));
    registerForm->middleNameLabel->setText(localeManager.translate(registerForm->middleNameLabel->text()));
    registerForm->registerButton->setText(localeManager.translate(registerForm->registerButton->text()));
    registerForm->infoLoginLink->setText(localeManager.translate(registerForm->infoLoginLink->text()));
    registerForm->loginLink->setText(localeManager.translate(registerForm->loginLink->text()));
}

void Register::initIcons() {
    registerForm->helpButton->setIcon(QIcon(":/res/assets/images/help.png"));
    registerForm->helpButton->setIconSize(QSize(16, 16));
}

void Register::checkFields() {
    if (!registerForm->loginField->text().isEmpty() &&
            !registerForm->passwordField->text().isEmpty() &&
            !registerForm->password2Field->text().isEmpty() &&
            !registerForm->firstNameField->text().isEmpty() &&
            !registerForm->secondNameField->text().isEmpty() &&
            !registerForm->middleNameField->text().isEmpty()) {
        registerForm->registerButton->setEnabled(true);
    } else {
        registerForm->registerButton->setEnabled(false);
    }
}

void Register::setSuccess(QLabel *status) {
    status->setText(QChar(0x2714));

    status->setFont(QFont("Times", 16));

    QPalette palette = status->palette();
    palette.setColor(QPalette::WindowText, QColor(0, 179, 0));
    status->setPalette(palette);
}

void Register::setError(QLabel *status) {
    status->setText(QChar(0x2718));

    status->setFont(QFont("Times", 16));

    QPalette palette = status->palette();
    palette.setColor(QPalette::WindowText, QColor(204, 0, 0));
    status->setPalette(palette);
}

void Register::Mouse_Pressed() {
    this->close();
    Auth *authForm = new Auth();
    authForm->show();
}

void Register::on_loginField_textEdited(const QString &arg1) {
    checkFields();

    QString loginValue = arg1;

    //Check for allowed symbols
    for (int i = 0; i < loginValue.length(); i++) {
        if (i == 0) {
            loginValue.replace(0, 1, '+');
            continue;
        }
        if (!loginValue.at(i).isDigit()) {
            loginValue.remove(i, 1);
        }
    }

    if (registerForm->loginField->text() != loginValue) {
        registerForm->loginField->setText(loginValue);
    }
}

void Register::on_loginField_editingFinished() {
    QString loginValue = registerForm->loginField->text();

    if (isPhoneValid(loginValue)) {
        try {
            if (!sqlManager.isUserExists(loginValue)) {
                setSuccess(registerForm->loginStatus);
                return;
            }
        } catch (const char* exception) {
            NotifyManager notify(CRITICAL);
            notify.showNotify(localeManager.translate(exception));
        }
    }

    setError(registerForm->loginStatus);
}


void Register::on_passwordField_textEdited(const QString &arg1) {
    checkFields();
}

void Register::on_passwordField_editingFinished() {
    QString passwordValue = registerForm->passwordField->text();

    if (isPasswordValid(passwordValue)) {
        setSuccess(registerForm->passwordStatus);
    } else {
        setError(registerForm->passwordStatus);
    }
}


void Register::on_password2Field_textEdited(const QString &arg1) {
    checkFields();
}

void Register::on_password2Field_editingFinished() {
    QString passwordValue = registerForm->passwordField->text();
    QString password2Value = registerForm->password2Field->text();

    if (isPasswordValid(passwordValue)) {
        if (passwordValue == password2Value) {
            setSuccess(registerForm->password2Status);
            return;
        }
    }

    setError(registerForm->password2Status);
}


void Register::on_firstNameField_textEdited(const QString &arg1) {
    checkFields();
}

void Register::on_firstNameField_editingFinished() {
    QString firstNameValue = registerForm->firstNameField->text();

    if (isPersonalNameValid(firstNameValue)) {
        setSuccess(registerForm->firstNameStatus);
    } else {
        setError(registerForm->firstNameStatus);
    }
}


void Register::on_secondNameField_textEdited(const QString &arg1) {
    checkFields();
}

void Register::on_secondNameField_editingFinished() {
    QString secondNameValue = registerForm->secondNameField->text();

    if (isPersonalNameValid(secondNameValue)) {
        setSuccess(registerForm->secondNameStatus);
    } else {
        setError(registerForm->secondNameStatus);
    }
}


void Register::on_middleNameField_textEdited(const QString &arg1) {
    checkFields();
}

void Register::on_middleNameField_editingFinished() {
    QString middleNameValue = registerForm->middleNameField->text();

    if (isPersonalNameValid(middleNameValue)) {
        setSuccess(registerForm->middleNameStatus);
    } else {
        setError(registerForm->middleNameStatus);
    }
}

void Register::on_registerButton_clicked() {
    //Check phone
    QString loginValue = registerForm->loginField->text();
    if (!isPhoneValid(loginValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidphone"));
        return;
    }

    //Check phone duplicates
    try {
        if (sqlManager.isUserExists(loginValue)) {
            NotifyManager notify(ERROR);
            notify.showNotify(localeManager.translate("msg.general.phoneregistered").replace("%phone%", loginValue));
            return;
        }
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    //Check password
    QString passwordValue = registerForm->passwordField->text();
    if (!isPasswordValid(passwordValue)) {
        NotifyManager notify(WARN);
        notify.showNotify(localeManager.translate("msg.general.weakpassword"));
        return;
    }

    //Check passwords match
    QString password2Value = registerForm->password2Field->text();
    if (passwordValue != password2Value) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.passwordnotmatch"));
        return;
    }

    //Check first name
    QString firstNameValue = registerForm->firstNameField->text();
    if (!isPersonalNameValid(firstNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidfirstname"));
        return;
    }

    //Check second name
    QString secondNameValue = registerForm->secondNameField->text();
    if (!isPersonalNameValid(secondNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidsecondname"));
        return;
    }

    //Check middle name
    QString middleNameValue = registerForm->middleNameField->text();
    if (!isPersonalNameValid(middleNameValue)) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidmiddlename"));
        return;
    }

    try {
        sqlManager.registerUser(loginValue, passwordValue, firstNameValue, secondNameValue, middleNameValue);
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    NotifyManager notify;
    notify.showNotify(localeManager.translate("msg.user.accountregistered"));

    this->close();
    Auth *authForm = new Auth(loginValue);
    authForm->show();
}

void Register::on_helpButton_clicked() {
    WindowManager(localeManager.translate("form.title.help"), localeManager.translate("help.register"));
}
