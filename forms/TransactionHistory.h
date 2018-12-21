#ifndef TRANSACTIONHISTORY_H
#define TRANSACTIONHISTORY_H

#include <QDialog>
#include <QCalendarWidget>

#include "managers/LogManager.h"

#include "utils/TimeUtils.h"
#include "utils/StringUtils.h"

namespace Ui {
    class TransactionHistory;
}

class TransactionHistory : public QDialog {
    Q_OBJECT

public:
    explicit TransactionHistory(QString walletId, QWidget *parent = nullptr);
    ~TransactionHistory();

private slots:
    void showData(int type = 0, long periodFrom = 0, long periodTo = 0);
    void on_searchButton_clicked();

private:
    Ui::TransactionHistory *transactionHistoryForm;
    LogManager logManager;

    void applyLocale();
    void initIcons();
    void initFilter();
};

#endif
