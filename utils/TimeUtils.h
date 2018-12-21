#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <ctime>
#include <QString>
#include <QDate>

using std::time;

namespace TimeUtils {
    long getCurrentTimestamp();
    QString getDate(long timestamp);
    QDate getQDate(long timestamp);
    QString getTime(long timestamp);
    long getTimestamp(int day, int month, int year, int second = 0, int minute = 0, int hour = 0);
}

#endif
