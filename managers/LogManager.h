#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <vector>

#include "classes/TransactionLog.h"

using std::vector;

class LogManager {
private:
    QString path;
    vector<TransactionLog> logs;
public:
    LogManager();
    LogManager(QString walletId);
    void addLog(TransactionLog log);
    void readLogs();
    void writeLogs();
    vector<TransactionLog> getLogs(int type = 0, long periodFrom = 0, long periodTo = 0);
    void descendingOrderSort();
};

#endif
