#ifndef USERCLIENT_H
#define USERCLIENT_H

#include <QWidget>
#include <QClipboard>

namespace Ui {
    class UserClient;
}

class UserClient : public QWidget {
    Q_OBJECT

public:
    explicit UserClient(QWidget *parent = nullptr);

    void updateData();

    ~UserClient();

private slots:
    void on_changePasswordButton_clicked();
    void on_logOutButton_clicked();
    void on_transferMoneyButton_clicked();
    void on_transactionsButton_clicked();
    void on_copyWalletIdButton_clicked();
    void on_helpButton_clicked();

private:
    Ui::UserClient *clientForm;

    void applyLocale();
    void initIcons();
};

#endif
