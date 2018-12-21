#ifndef TRANSACTIONLOG_H
#define TRANSACTIONLOG_H

#include <QString>

#include "classes/TransactionType.h"

class TransactionLog {
private:
    TransactionType type;
    long timestamp;
    QString value;
    double amount;
public:
    TransactionLog();
    TransactionLog(TransactionType type, long timestamp, QString value, double amount);
    int getType();
    long getTimestamp();
    QString getValue();
    double getAmount();
};

#endif
