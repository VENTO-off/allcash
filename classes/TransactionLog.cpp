#include "TransactionLog.h"

TransactionLog::TransactionLog() {}

TransactionLog::TransactionLog(TransactionType type, long timestamp, QString value, double amount) {
    this->type = type;
    this->timestamp = timestamp;
    this->value = value;
    this->amount = amount;
}

int TransactionLog::getType() {
    return this->type;
}

long TransactionLog::getTimestamp() {
    return this->timestamp;
}

QString TransactionLog::getValue() {
    return this->value;
}

double TransactionLog::getAmount() {
    return this->amount;
}
