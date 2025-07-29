#include "plugin.h"
#include "RemoteInstallerWidget.h"

#include <QtQml>

void RemoteInstallerPlugin::registerTypes (const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.private.remoteinstaller"));
    qmlRegisterType<RemoteInstallerWidget>(uri, 1, 0, "RemoteInstallerWidget");
}
