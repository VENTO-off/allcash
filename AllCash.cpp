#include <QApplication>

#include "forms/Auth.h"

#include "classes/User.h"

#include "managers/SqlManager.h"
#include "managers/NotifyManager.h"
#include "managers/LocaleManager.h"
#include "managers/SettingsManager.h"

/*    GLOBAL VARIABLES    */
User *loggedAccount;
SqlManager sqlManager;
LocaleManager localeManager;
SettingsManager settingsManager;
/**************************/

void closeConnection(void) {
    sqlManager.disconnect();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    settingsManager.load();

    localeManager.init();

    try {
        sqlManager.connect();
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    try {
        if (!sqlManager.checkVersion()) {
            NotifyManager notify(ERROR);
            notify.showNotify(localeManager.translate("msg.general.newversion"));
            return -2;
        }
    } catch (const char* exception) {
        NotifyManager notify(CRITICAL);
        notify.showNotify(localeManager.translate(exception));
    }

    atexit(closeConnection);

    Auth w;
    w.show();

    return a.exec();
}
