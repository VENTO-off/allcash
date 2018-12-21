#include "utils/TimeUtils.h"

namespace TimeUtils {
    long getCurrentTimestamp() {
        time_t t = time(0);
        return t;
    }

    QString getDate(long timestamp) {
        time_t now(timestamp);
        tm *time = localtime(&now);

        QString day = QString::number(time->tm_mday);
        QString month = QString::number(1 + time->tm_mon);
        QString year =  QString::number(1900 + time->tm_year);

        return (day.length() == 1 ? "0" : "") + day + "." + (month.length() == 1 ? "0" : "") + month + "." + (year.length() == 1 ? "0" : "") + year;
    }

    QDate getQDate(long timestamp) {
        time_t now(timestamp);
        tm *time = localtime(&now);

        return QDate(1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday);
    }

    QString getTime(long timestamp) {
        time_t now(timestamp);
        tm *time = localtime(&now);

        QString hour = QString::number(time->tm_hour);
        QString minute = QString::number(time->tm_min);
        QString second = QString::number(time->tm_sec);

        return (hour.length() == 1 ? "0" + hour : hour) + ":" + (minute.length() == 1 ? "0" + minute : minute) + ":" + (second.length() == 1 ? "0" + second : second);
    }

    long getTimestamp(int day, int month, int year, int second, int minute, int hour) {
        time_t now = time(0);
        tm *time = localtime(&now);

        time->tm_sec = second;
        time->tm_min = minute;
        time->tm_hour = hour;
        time->tm_mday = day;
        time->tm_mon = month - 1;
        time->tm_year = year - 1900;

        return mktime(time);
    }
}
