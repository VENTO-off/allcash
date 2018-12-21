#include "ui_UserClient.h"

#include "forms/UserClient.h"
#include "forms/ChangePassword.h"
#include "forms/TransactionHistory.h"
#include "forms/TransferMoney.h"

#include "classes/User.h"

#include "managers/SqlManager.h"
#include "managers/NotifyManager.h"
#include "managers/LocaleManager.h"
#include "managers/WindowManager.h"

#include "utils/StringUtils.h"

using StringUtils::removeHtmlTags;
using StringUtils::formatAmount;

extern User * loggedAccount;
extern SqlManager sqlManager;
extern LocaleManager localeManager;

UserClient::UserClient(QWidget *parent) : QWidget(parent), clientForm(new Ui::UserClient) {
    clientForm->setupUi(this);
    this->setFixedSize(this->size());

    applyLocale();
    initIcons();
    updateData();
}

void UserClient::updateData() {
    if (loggedAccount != 0) {
        QString current_balance = removeHtmlTags(clientForm->balanceValue->text());
        clientForm->balanceValue->setText(clientForm->balanceValue->text().replace(current_balance, formatAmount(static_cast<DefaultUser*>(loggedAccount)->getBalance())));
        clientForm->walletIdLabel->setText(clientForm->walletIdLabel->text().replace("%walletId%", static_cast<DefaultUser*>(loggedAccount)->getWalletId()));
    }
}

UserClient::~UserClient() {
    delete clientForm;
}

void UserClient::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    clientForm->welcomeLabel->setText(localeManager.translate(clientForm->welcomeLabel->text()).replace("%name%", loggedAccount->getFirstName() + " " + loggedAccount->getSecondName()));
    clientForm->balanceBox->setTitle(localeManager.translate(clientForm->balanceBox->title()));
    clientForm->walletBox->setTitle(localeManager.translate(clientForm->walletBox->title()));
    clientForm->accountBox->setTitle(localeManager.translate(clientForm->accountBox->title()));
    clientForm->changePasswordButton->setText(localeManager.translate(clientForm->changePasswordButton->text()));
    clientForm->logOutButton->setText(localeManager.translate(clientForm->logOutButton->text()));
    clientForm->transactionsBox->setTitle(localeManager.translate(clientForm->transactionsBox->title()));
    clientForm->transferMoneyButton->setText(localeManager.translate(clientForm->transferMoneyButton->text()));
    clientForm->transactionsButton->setText(localeManager.translate(clientForm->transactionsButton->text()));
    localeManager.showLocales(clientForm->localeChanger);
}

void UserClient::initIcons() {
    clientForm->copyWalletIdButton->setIcon(QIcon(":/res/assets/images/copy.png"));
    clientForm->copyWalletIdButton->setIconSize(QSize(39, 39));
    clientForm->changePasswordButton->setIcon(QIcon(":/res/assets/images/password.png"));
    clientForm->changePasswordButton->setIconSize(QSize(16, 16));
    clientForm->logOutButton->setIcon(QIcon(":/res/assets/images/logout.png"));
    clientForm->logOutButton->setIconSize(QSize(16, 16));
    clientForm->transferMoneyButton->setIcon(QIcon(":/res/assets/images/transfer.png"));
    clientForm->transferMoneyButton->setIconSize(QSize(16, 16));
    clientForm->transactionsButton->setIcon(QIcon(":/res/assets/images/history.png"));
    clientForm->transactionsButton->setIconSize(QSize(16, 16));
    clientForm->helpButton->setIcon(QIcon(":/res/assets/images/help.png"));
    clientForm->helpButton->setIconSize(QSize(16, 16));
}

void UserClient::on_changePasswordButton_clicked() {
    ChangePassword *changePassword = new ChangePassword();
    changePassword->setModal(true);
    changePassword->show();
}

void UserClient::on_logOutButton_clicked() {
    this->close();
    loggedAccount->logOut();
    loggedAccount = 0;
}

void UserClient::on_transferMoneyButton_clicked() {
    TransferMoney *transferMoney = new TransferMoney(this);
    transferMoney->setModal(true);
    transferMoney->show();
}

void UserClient::on_transactionsButton_clicked() {
    TransactionHistory *transactionHistory = new TransactionHistory(static_cast<DefaultUser*>(loggedAccount)->getWalletId());
    transactionHistory->setModal(true);
    transactionHistory->show();
}

void UserClient::on_copyWalletIdButton_clicked() {
    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(static_cast<DefaultUser*>(loggedAccount)->getWalletId());
    NotifyManager notify;
    notify.showNotify(localeManager.translate("msg.user.walletcopied"));
}

void UserClient::on_helpButton_clicked() {
    WindowManager(localeManager.translate("form.title.help"), localeManager.translate("help.userclient"));
}
