#include <QDir>
#include <QSettings>
#include <QTextStream>

#include "managers/SettingsManager.h"

SettingsManager::SettingsManager() {}

void SettingsManager::load() {
    QDir settingsDir("settings");
    if (!settingsDir.exists()) {
        QDir().mkdir(settingsDir.dirName());
    }
    path = settingsDir.dirName() + "/" + "allcash.cfg";

    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup("/accounts");
    this->lastAccount = settings.value("/last", "").toString();
    settings.endGroup();

    settings.beginGroup("/locale");
    this->language = settings.value("/language", "en_US").toString();
    settings.endGroup();
}

void SettingsManager::save() {
    QDir settingsDir("settings");
    if (!settingsDir.exists()) {
        QDir().mkdir(settingsDir.dirName());
    }

    QSettings settings(path, QSettings::IniFormat);

    settings.beginGroup("/accounts");
    settings.setValue("/last", lastAccount);
    settings.endGroup();

    settings.beginGroup("/locale");
    settings.setValue("/language", language);
    settings.endGroup();

    settings.sync();
}

QString SettingsManager::getLastAccount() {
    return this->lastAccount;
}

QString SettingsManager::getLanguage() {
    return this->language;
}

void SettingsManager::setLastAccount(QString lastAccount) {
    this->lastAccount = lastAccount;
}

void SettingsManager::setLanguage(QString language) {
    this->language = language;
}
