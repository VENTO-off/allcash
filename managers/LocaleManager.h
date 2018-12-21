#ifndef LOCALEMANAGER_H
#define LOCALEMANAGER_H

#include <QComboBox>
#include <QString>
#include <map>

#include <classes/Locale.h>

using std::map;

class LocaleManager {
private:
    map<int, Locale> locales;
    map<QString, QString> locale;
public:
    LocaleManager();
    void init();
    void loadLocales();
    QString parseLocale(QString lang, QString value = QString());
    void showLocales(QComboBox *comboBox);
    QString translate(QString internalName);
};

#endif
