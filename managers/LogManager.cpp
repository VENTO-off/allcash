#include <QDir>
#include <fstream>

#include "managers/LogManager.h"
#include "managers/NotifyManager.h"

#include "libs/json/jsoncpp.cpp"

using std::ifstream;
using std::ofstream;
using std::ios;
using std::sort;
using Json::Reader;
using Json::Value;
using Json::StyledStreamWriter;

LogManager::LogManager() {}

LogManager::LogManager(QString walletId) {
    QDir logsDir("logs");
    if (!logsDir.exists()) {
        QDir().mkdir(logsDir.dirName());
    }
    path = logsDir.dirName() + "/" + walletId + ".json";
}

void LogManager::addLog(TransactionLog log) {
    logs.push_back(log);
}

void LogManager::readLogs() {
    QFile logsFile(path);
    if (!logsFile.exists()) {
        return;
    }

    try {
        ifstream ifs(path.toStdString(), ios::in);
        ifs.is_open();

        Reader reader;
        Value loaded_logs;
        reader.parse(ifs, loaded_logs);

        for (int i = 0; i < loaded_logs.size(); i++) {
            TransactionLog log = TransactionLog(static_cast<TransactionType>(loaded_logs[i]["type"].asInt()),
                    loaded_logs[i]["timestamp"].asLargestInt(),
                    QString::fromStdString(loaded_logs[i]["value"].asString()),
                    loaded_logs[i]["amount"].asDouble());
            logs.push_back(log);
        }

        ifs.close();
    } catch (...) {
        logsFile.remove();
        return;
    }
}

void LogManager::writeLogs() {
    try {
        Value writable_logs;

        for (int i = 0; i < logs.size(); i++) {
            TransactionLog log = logs[i];
            writable_logs[i]["type"] = Json::Int(log.getType());
            writable_logs[i]["timestamp"] = Json::LargestInt(log.getTimestamp());
            writable_logs[i]["value"] = log.getValue().toStdString();
            writable_logs[i]["amount"] = log.getAmount();
        }

        ofstream ofs(path.toStdString(), ios::out | ios::trunc);
        StyledStreamWriter stream;
        stream.write(ofs, writable_logs);

        ofs.close();
    } catch (...) {
        throw "msg.general.errorwritefile";
    }
}

vector<TransactionLog> LogManager::getLogs(int type, long periodFrom, long periodTo) {
    if (!type && !periodFrom && !periodFrom) {
        return logs;
    }

    vector<TransactionLog> filteredLogs;

    for (TransactionLog log : logs) {
        if (log.getTimestamp() >= periodFrom && log.getTimestamp() <= periodTo) {
            if (type) {
                if (log.getType() != static_cast<TransactionType>(type)) {
                    continue;
                }
            }

            filteredLogs.push_back(log);
        }
    }

    return filteredLogs;
}

void LogManager::descendingOrderSort() {
    struct {
        bool operator()(TransactionLog log1, TransactionLog log2) const {
            return log1.getTimestamp() > log2.getTimestamp();
        }
    } descendingOrder;
    sort(logs.begin(), logs.end(), descendingOrder);
}
