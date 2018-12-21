#include "ui_ConfirmTransferMoney.h"

#include "forms/ConfirmTransferMoney.h"

#include "managers/MoneyManager.h"
#include "managers/NotifyManager.h"
#include "managers/LocaleManager.h"

#include "utils/StringUtils.h"

using StringUtils::formatAmount;

extern LocaleManager localeManager;

ConfirmTransferMoney::ConfirmTransferMoney(UserClient *userClient, QString toWallet, QString recipient, double amount, QWidget *parent) : QDialog(parent), confirmTransferMoneyForm(new Ui::ConfirmTransferMoney) {
    confirmTransferMoneyForm->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->clientForm = userClient;
    this->toWallet = toWallet;
    this->recipient = recipient;
    this->amount = amount;

    applyLocale();
    updateData();
}

ConfirmTransferMoney::~ConfirmTransferMoney() {
    delete confirmTransferMoneyForm;
}

void ConfirmTransferMoney::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    confirmTransferMoneyForm->title->setText(localeManager.translate(confirmTransferMoneyForm->title->text()));
    confirmTransferMoneyForm->title2->setText(localeManager.translate(confirmTransferMoneyForm->title2->text()));
    confirmTransferMoneyForm->walletLabel->setText(localeManager.translate(confirmTransferMoneyForm->walletLabel->text()));
    confirmTransferMoneyForm->recipientLabel->setText(localeManager.translate(confirmTransferMoneyForm->recipientLabel->text()));
    confirmTransferMoneyForm->amountLabel->setText(localeManager.translate(confirmTransferMoneyForm->amountLabel->text()));
    confirmTransferMoneyForm->confirmButton->setText(localeManager.translate(confirmTransferMoneyForm->confirmButton->text()));
    confirmTransferMoneyForm->cancelButton->setText(localeManager.translate(confirmTransferMoneyForm->cancelButton->text()));
}

void ConfirmTransferMoney::updateData() {
    confirmTransferMoneyForm->walletField->setText(confirmTransferMoneyForm->walletField->text().replace("%walletId%", toWallet));
    confirmTransferMoneyForm->recipientField->setText(confirmTransferMoneyForm->recipientField->text().replace("%recipient%", recipient));
    confirmTransferMoneyForm->amountField->setText(confirmTransferMoneyForm->amountField->text().replace("%amount%", formatAmount(amount)));
}

void ConfirmTransferMoney::on_confirmButton_clicked() {
    try {
        MoneyManager().sendMoney(amount, toWallet);
    } catch (const char* exception) {
        NotifyManager notify(ERROR);
        notify.showNotify(localeManager.translate(exception));
        return;
    }

    this->close();

    clientForm->updateData();

    NotifyManager notify;
    notify.showNotify(localeManager.translate("msg.money.moneytransfered").replace("%amount%", formatAmount(amount)).replace("%wallet%", toWallet));
}

void ConfirmTransferMoney::on_cancelButton_clicked() {
    this->close();
}
