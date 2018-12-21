#include "ui_TransactionHistory.h"

#include "forms/TransactionHistory.h"

#include "managers/LocaleManager.h";

using StringUtils::formatAmount;
using TimeUtils::getQDate;
using TimeUtils::getDate;
using TimeUtils::getTime;
using TimeUtils::getTimestamp;

extern LocaleManager localeManager;

TransactionHistory::TransactionHistory(QString walletId, QWidget *parent) : QDialog(parent), transactionHistoryForm(new Ui::TransactionHistory) {
    transactionHistoryForm->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    logManager = LogManager(walletId);
    logManager.readLogs();
    logManager.descendingOrderSort();

    applyLocale();
    initIcons();
    initFilter();
    showData();
}

TransactionHistory::~TransactionHistory() {
    delete transactionHistoryForm;
}

void TransactionHistory::applyLocale() {
    this->setWindowTitle("AllCash :: " + localeManager.translate(this->windowTitle()));
    transactionHistoryForm->title->setText(localeManager.translate(transactionHistoryForm->title->text()));
    transactionHistoryForm->filterGroupBox->setTitle(localeManager.translate(transactionHistoryForm->filterGroupBox->title()));
    transactionHistoryForm->period1Label->setText(localeManager.translate(transactionHistoryForm->period1Label->text()));
    transactionHistoryForm->period2Label->setText(localeManager.translate(transactionHistoryForm->period2Label->text()));
    transactionHistoryForm->operationLabel->setText(localeManager.translate(transactionHistoryForm->operationLabel->text()));
    for (int i = 0; i < transactionHistoryForm->operationComboBox->count(); i++) {
        transactionHistoryForm->operationComboBox->setItemText(i, localeManager.translate(transactionHistoryForm->operationComboBox->itemText(i)));
    }
    transactionHistoryForm->searchButton->setText(localeManager.translate(transactionHistoryForm->searchButton->text()));
    for (int i = 0; i < transactionHistoryForm->transactionsTable->columnCount(); i++) {
        transactionHistoryForm->transactionsTable->horizontalHeaderItem(i)->setText(localeManager.translate(transactionHistoryForm->transactionsTable->horizontalHeaderItem(i)->text()));
    }
}

void TransactionHistory::initIcons() {
    transactionHistoryForm->searchButton->setIcon(QIcon(":/res/assets/images/search.png"));
    transactionHistoryForm->searchButton->setIconSize(QSize(24, 24));
}

void TransactionHistory::initFilter() {
    if (logManager.getLogs().empty()) {
        transactionHistoryForm->filterGroupBox->setEnabled(false);
        return;
    }

    long periodFrom = logManager.getLogs().back().getTimestamp();
    transactionHistoryForm->periodFrom->setDate(getQDate(periodFrom));

    transactionHistoryForm->periodTo->setDate(QDate::currentDate());
}

void TransactionHistory::showData(int type, long periodFrom, long periodTo) {
    vector<TransactionLog> logs = logManager.getLogs(type, periodFrom, periodTo);

    transactionHistoryForm->transactionsTable->setRowCount(logs.size());

    for (int i = 0; i < logs.size(); i++) {
        TransactionLog log = logs[i];
        transactionHistoryForm->transactionsTable->setItem(i, 0, new QTableWidgetItem(getDate(log.getTimestamp())));
        transactionHistoryForm->transactionsTable->setItem(i, 1, new QTableWidgetItem(getTime(log.getTimestamp())));
        transactionHistoryForm->transactionsTable->setItem(i, 2, new QTableWidgetItem((log.getType() == INCOME ? "+" : "-") + formatAmount(log.getAmount())));
        transactionHistoryForm->transactionsTable->setItem(i, 3, new QTableWidgetItem(log.getValue().replace("%transfer_to%", localeManager.translate("msg.money.transferto"))
                                                                                                    .replace("%transfer_from%", localeManager.translate("msg.money.transferfrom"))));
    }
}

void TransactionHistory::on_searchButton_clicked() {
    QDate periodFrom = transactionHistoryForm->periodFrom->date();
    QDate periodTo = transactionHistoryForm->periodTo->date();

    showData(transactionHistoryForm->operationComboBox->currentIndex(),
             getTimestamp(periodFrom.day(), periodFrom.month(), periodFrom.year()),
             getTimestamp(periodTo.day(), periodTo.month(), periodTo.year(), 59, 59, 23));
}
