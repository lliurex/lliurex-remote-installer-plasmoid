#ifndef REMOTE_INSTALLER_PLUGIN_H
#define REMOTE_INSTALLER_PLUGIN_H

#include <QQmlEngine>
#include <QQmlExtensionPlugin>

class RemoteInstallerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
/*    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")*/
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) Q_DECL_OVERRIDE;
};

#endif // REMOTE_INSTALLER_PLUGIN_H
