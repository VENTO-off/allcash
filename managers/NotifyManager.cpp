#include "NotifyManager.h"

#include "managers/LocaleManager.h"

extern LocaleManager localeManager;

NotifyManager::NotifyManager(NotifyType type) {
    this->type = type;
    if (type == SUCCESS) {
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle("AllCash :: " + localeManager.translate("msg.title.success"));
    } else if (type == WARN) {
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("AllCash :: " + localeManager.translate("msg.title.warning"));
    } else if (type == ERROR) {
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("AllCash :: " + localeManager.translate("msg.title.error"));
    } else if (type == CRITICAL) {
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("AllCash :: " + localeManager.translate("msg.title.critical"));
        msg.setInformativeText(localeManager.translate("msg.title.terminate"));
    }
}

void NotifyManager::showNotify(QString message) {
    msg.setText(message.replace("\\n", "<br>"));
    msg.exec();

    if (type == CRITICAL) {
        exit(-1);
    }
}
