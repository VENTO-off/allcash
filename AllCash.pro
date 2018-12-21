#-------------------------------------------------
#
# Project created by QtCreator 2018-10-12T22:50:49
#
#-------------------------------------------------

QT       += core gui widgets sql network

TARGET = AllCash
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
    AllCash.cpp \
    forms/Auth.cpp \
    classes/TransactionLog.cpp \
    managers/LogManager.cpp \
    managers/SqlManager.cpp \
    libs/md5/md5.cpp \
    libs/base64/base64.cpp \
    classes/User.cpp \
    classes/DefaultUser.cpp \
    forms/Register.cpp \
    classes/ClickableQLabel.cpp \
    forms/UserClient.cpp \
    forms/ChangePassword.cpp \
    managers/MoneyManager.cpp \
    forms/TransferMoney.cpp \
    forms/ConfirmTransferMoney.cpp \
    forms/TransactionHistory.cpp \
    classes/AdminUser.cpp \
    forms/AdminClient.cpp \
    managers/NotifyManager.cpp \
    forms/EditUser.cpp \
    forms/AddAdmin.cpp \
    utils/StringUtils.cpp \
    utils/TimeUtils.cpp \
    managers/LocaleManager.cpp \
    classes/Locale.cpp \
    managers/SettingsManager.cpp \
    managers/WindowManager.cpp

HEADERS += \
    forms/Auth.h \
    classes/TransactionLog.h \
    managers/LogManager.h \
    managers/SqlManager.h \
    SqlConfig.h \
    libs/md5/md5.h \
    libs/base64/base64.h \
    classes/User.h \
    utils/TimeUtils.h \
    classes/DefaultUser.h \
    forms/Register.h \
    classes/ClickableQLabel.h \
    forms/UserClient.h \
    forms/ChangePassword.h \
    utils/StringUtils.h \
    managers/MoneyManager.h \
    forms/TransferMoney.h \
    forms/ConfirmTransferMoney.h \
    classes/TransactionType.h \
    forms/TransactionHistory.h \
    classes/AdminUser.h \
    forms/AdminClient.h \
    managers/NotifyManager.h \
    forms/EditUser.h \
    forms/AddAdmin.h \
    managers/LocaleManager.h \
    classes/Locale.h \
    managers/SettingsManager.h \
    managers/WindowManager.h

FORMS += \
    forms/Auth.ui \
    forms/Register.ui \
    forms/UserClient.ui \
    forms/ChangePassword.ui \
    forms/TransferMoney.ui \
    forms/ConfirmTransferMoney.ui \
    forms/TransactionHistory.ui \
    forms/AdminClient.ui \
    forms/EditUser.ui \
    forms/AddAdmin.ui

INCLUDEPATH = $$PWD/libs

QMAKE_CXXFLAGS += -Wno-deprecated

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
