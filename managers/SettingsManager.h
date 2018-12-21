#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>
#include <QString>

class SettingsManager {
private:
    QString path;
    QString lastAccount;
    QString language;
public:
    SettingsManager();
    void load();
    void save();
    QString getLastAccount();
    QString getLanguage();
    void setLastAccount(QString lastAccount);
    void setLanguage(QString language);
};

#endif
