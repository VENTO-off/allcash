#include "Locale.h"

Locale::Locale(QString internalName, QString displayName, QString pathToIcon, QString applyLocaleMsg) {
    this->internalName = internalName;
    this->displayName = displayName;
    this->pathToIcon = pathToIcon;
    this->applyLocaleMsg = applyLocaleMsg;
}

QString Locale::getInternalName() {
    return this->internalName;
}

QString Locale::getDisplayName() {
    return this->displayName;
}

QString Locale::getPathToIcon() {
    return this->pathToIcon;
}

QString Locale::getApplyLocaleMsg() {
    return this->applyLocaleMsg;
}
