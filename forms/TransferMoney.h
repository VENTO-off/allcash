#ifndef TRANSFERMONEY_H
#define TRANSFERMONEY_H

#include <QDialog>

#include "forms/UserClient.h"

namespace Ui {
    class TransferMoney;
}

class TransferMoney : public QDialog {
    Q_OBJECT

public:
    explicit TransferMoney(UserClient *userClient, QWidget *parent = nullptr);
    ~TransferMoney();

private slots:
    void on_walletIdField_textEdited(const QString &arg1);
    void on_amountField_textEdited(const QString &arg1);
    void on_payButton_clicked();

private:
    Ui::TransferMoney *transferMoneyForm;
    UserClient *clientForm;

    void applyLocale();
    void checkFields();
    void checkAmount();
    void checkWallet();
};

#endif
