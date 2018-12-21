#include "ui_TransferMoney.h"

#include "forms/ConfirmTransferMoney.h"
#include "forms/TransferMoney.h"

#include "managers/MoneyManager.h"
#include "managers/NotifyManager.h"
#include "managers/SqlManager.h"
#include "managers/LocaleManager.h"

#include "utils/StringUtils.h"

using StringUtils::isAmountValid;
using StringUtils::isWalletValid;

extern User *loggedAccount;
extern SqlManager sqlManager;
extern LocaleManager localeManager;

TransferMoney::TransferMoney(UserClient *userClient, QWidget *parent) : QDialog(parent), transferMoneyForm(new Ui::TransferMoney) {
    transferMoneyForm->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    clientForm = userClient;

    applyLocale();
}

TransferMoney::~TransferMoney() {
    delete transferMoneyForm;
}

void TransferMoney::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    transferMoneyForm->title->setText(localeManager.translate(transferMoneyForm->title->text()));
    transferMoneyForm->title2->setText(localeManager.translate(transferMoneyForm->title2->text()));
    transferMoneyForm->walletIdLabel->setText(localeManager.translate(transferMoneyForm->walletIdLabel->text()));
    transferMoneyForm->amountLabel->setText(localeManager.translate(transferMoneyForm->amountLabel->text()));
    transferMoneyForm->payButton->setText(localeManager.translate(transferMoneyForm->payButton->text()));
}

void TransferMoney::checkFields() {
    if (!transferMoneyForm->walletIdField->text().isEmpty() && !transferMoneyForm->amountField->text().isEmpty()) {
        transferMoneyForm->payButton->setEnabled(true);
    } else {
        transferMoneyForm->payButton->setEnabled(false);
    }
}

void TransferMoney::checkAmount() {
    QString moneyValue = transferMoneyForm->amountField->text();

    //Check for allowed symbols
    for (int i = 0; i < moneyValue.length(); i++) {
        if (!moneyValue.at(i).isDigit() && moneyValue.at(i) != '.') {
            moneyValue.remove(i, 1);
        }
    }

    //Check for more than one dots
    bool hasDot = false;
    for (int i = 0; i < moneyValue.length(); i++) {
        if (moneyValue.at(i) == '.' && !hasDot) {
            hasDot = true;
        } else if (moneyValue.at(i) == '.' && hasDot) {
            moneyValue.remove(i, 1);
        }
    }

    //Check for valid money amount
    if (moneyValue.contains('.')) {
        if ((moneyValue.indexOf('.') + 3) < moneyValue.length()) {
            moneyValue.remove(moneyValue.indexOf('.') + 3, moneyValue.length());
        }
    }

    if (transferMoneyForm->amountField->text() != moneyValue) {
        transferMoneyForm->amountField->setText(moneyValue);
    }
}

void TransferMoney::checkWallet() {
    QString walletId = transferMoneyForm->walletIdField->text();

    //Check for allowed symbols
    for (int i = 0; i < walletId.length(); i++) {
        if (!walletId.at(i).isDigit()) {
            walletId.remove(i, 1);
        }
    }

    if (transferMoneyForm->walletIdField->text() != walletId) {
        transferMoneyForm->walletIdField->setText(walletId);
    }
}

void TransferMoney::on_walletIdField_textEdited(const QString &arg1) {
    checkFields();
    checkWallet();
}

void TransferMoney::on_amountField_textEdited(const QString &arg1) {
    checkFields();
    checkAmount();
}

void TransferMoney::on_payButton_clicked() {
    //Check wallet string
    if (!isWalletValid(transferMoneyForm->walletIdField->text())) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.general.invalidwallet"));
        return;
    }

    //Check user's wallet
    if (transferMoneyForm->walletIdField->text() == static_cast<DefaultUser*>(loggedAccount)->getWalletId()) {
        NotifyManager notify(WARN);
        notify.showNotify(localeManager.translate("msg.money.canttransfertoyourself"));
        return;
    }

    //Check money string
    if (!isAmountValid(transferMoneyForm->amountField->text())) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate("msg.money.invalidformat"));
        return;
    }

    QString toWallet = transferMoneyForm->walletIdField->text();
    QString recipient;
    try {
        recipient = sqlManager.getUserPersonalData(toWallet);
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }
    double amount = fabs(transferMoneyForm->amountField->text().toDouble());

    this->close();

    ConfirmTransferMoney *confirmTransferMoney = new ConfirmTransferMoney(clientForm, toWallet, recipient, amount);
    confirmTransferMoney->setModal(true);
    confirmTransferMoney->show();

}
