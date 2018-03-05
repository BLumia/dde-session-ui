
#include "dmemorywarningdialog.h"
#include "dmemorywarningdialogadaptor.h"

#include <DApplication>
#include <DLog>

#include <QDBusConnection>
#include <QTranslator>

DWIDGET_USE_NAMESPACE
DUTIL_USE_NAMESPACE

int main(int argc, char *args[])
{
    DApplication::loadDXcbPlugin();
    DApplication dapp(argc, args);
    dapp.setQuitOnLastWindowClosed(false);
    dapp.setAttribute(Qt::AA_UseHighDpiPixmaps);
    dapp.setTheme("light");

    if (!dapp.setSingleInstance("dmemory-warning-dialog"))
        return -1;

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    dapp.installTranslator(&translator);

#ifdef QT_DEBUG
    DLogManager::registerConsoleAppender();
#else
    DLogManager::registerFileAppender();
#endif

    DMemoryWarningDialog dialog;
    DMemoryWarningDialogAdaptor dbusAdaptor(&dialog);
    Q_UNUSED(dbusAdaptor);
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService("com.deepin.dde.MemoryWarningDialog") ||
        !connection.registerObject("/com/deepin/dde/MemoryWarningDialog", &dialog))
        return -1;

#ifdef QT_DEBUG
    dialog.show();
#endif

    return dapp.exec();
}
