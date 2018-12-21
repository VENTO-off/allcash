#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <QMessageBox>

enum NotifyType {
    SUCCESS,
    WARN,
    ERROR,
    CRITICAL
};

class NotifyManager {
private:
    QMessageBox msg;
    NotifyType type;
public:
    NotifyManager(NotifyType type = SUCCESS);
    void showNotify(QString message);
};

#endif
