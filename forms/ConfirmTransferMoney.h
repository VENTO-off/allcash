#ifndef CONFIRMTRANSFERMONEY_H
#define CONFIRMTRANSFERMONEY_H

#include <QDialog>

#include "forms/UserClient.h"

namespace Ui {
    class ConfirmTransferMoney;
}

class ConfirmTransferMoney : public QDialog {
    Q_OBJECT

public:
    explicit ConfirmTransferMoney(UserClient *userClient, QString toWallet, QString recipient, double amount, QWidget *parent = nullptr);
    ~ConfirmTransferMoney();

private slots:
    void on_confirmButton_clicked();   
    void on_cancelButton_clicked();

private:
    Ui::ConfirmTransferMoney *confirmTransferMoneyForm;
    UserClient *clientForm;
    QString toWallet;
    QString recipient;
    double amount;

    void applyLocale();
    void updateData();
};

#endif
