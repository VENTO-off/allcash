#ifndef LOCALE_H
#define LOCALE_H

#include <QString>

class Locale {
private:
    QString internalName;
    QString displayName;
    QString pathToIcon;
    QString applyLocaleMsg;
public:
    Locale(QString internalName, QString displayName, QString pathToIcon, QString applyLocaleMsg);
    QString getInternalName();
    QString getDisplayName();
    QString getPathToIcon();
    QString getApplyLocaleMsg();
};

#endif
