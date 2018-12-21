#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <iostream>

#include "managers/LocaleManager.h"
#include "managers/NotifyManager.h"
#include "managers/SettingsManager.h"

#include "utils/StringUtils.h"

using std::pair;
using std::cerr;
using std::endl;
using StringUtils::removeHtmlTags;

extern SettingsManager settingsManager;

LocaleManager::LocaleManager() {}

void LocaleManager::init() {
    loadLocales();

    try {
        parseLocale(settingsManager.getLanguage());
    } catch (const char* exception) {
        NotifyManager notify(ERROR);
        notify.showNotify(exception);
    }
}

void LocaleManager::loadLocales() {
    QDir directory(":/res/assets/lang/");
    QStringList localeList = directory.entryList(QStringList("*.lang"));

    for (int i = 0; i < localeList.size(); i++) {
        try {
            QString internalName = localeList[i].replace(".lang", "");
            QString displayName = parseLocale(internalName, "locale.displayname");
            QString pathToIcon = parseLocale(internalName, "locale.icon");
            QString applyLocaleMsg = parseLocale(internalName, "locale.applylocalemsg");
            locales.insert(pair<int, Locale>(i, Locale(internalName, displayName, pathToIcon, applyLocaleMsg)));
        } catch (const char* exception) {
            NotifyManager notify(ERROR);
            notify.showNotify(exception);
        }
    }
}

QString LocaleManager::parseLocale(QString lang, QString value) {
    try {
        QFile langFile(":/res/assets/lang/" + lang + ".lang");
        langFile.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream in(&langFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty() && !line.startsWith("#")) {
                QStringList current_line = line.split("=", QString::SkipEmptyParts);
                if (!value.isEmpty()) {
                    if (current_line.at(0) == value) {
                        return current_line.at(1);
                    }

                    continue;
                }
                locale.insert(pair<QString, QString>(current_line.at(0), current_line.at(1)));
            }
        }
        langFile.close();
    } catch (...) {
        throw "Error loading \"" + lang + "\" locale!";
    }

    return 0;
}

void LocaleManager::showLocales(QComboBox *comboBox) {
    map<int, Locale>::iterator it;
    for (it = locales.begin(); it != locales.end(); it++) {
        Locale locale = it->second;
        comboBox->addItem(QIcon(locale.getPathToIcon()), locale.getDisplayName());
        if (locale.getInternalName() == settingsManager.getLanguage()) {
            comboBox->setCurrentIndex(it->first);
        }
    }

    QObject::connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),[=](int index) {
        map<int, Locale>::iterator it = locales.find(index);
        if (it != locales.end()) {
            Locale locale = it->second;
            if (settingsManager.getLanguage() != locale.getInternalName()) {
                NotifyManager notify(WARN);
                notify.showNotify(locale.getApplyLocaleMsg());
                settingsManager.setLanguage(locale.getInternalName());
                settingsManager.save();
            }
        }
    });
}

QString LocaleManager::translate(QString internalName) {
    QString gui_name = removeHtmlTags(internalName);

    map<QString, QString>::iterator it = locale.find(gui_name);
    if (it != locale.end()) {
        return internalName.replace(it->first, it->second);
    }

    return internalName;
}
