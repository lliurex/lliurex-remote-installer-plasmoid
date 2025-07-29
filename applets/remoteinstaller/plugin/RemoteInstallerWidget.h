#ifndef PLASMA_REMOTE_INSTALLER_WIDGET_H
#define PLASMA_REMOTE_INSTALLER_WIDGET_H

#include <QObject>
#include <QProcess>
#include <QPointer>
#include <KNotification>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>


class QTimer;
class KNotification;


class RemoteInstallerWidget : public QObject
{
    Q_OBJECT


    Q_PROPERTY(TrayStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString toolTip READ toolTip NOTIFY toolTipChanged)
    Q_PROPERTY(QString subToolTip READ subToolTip NOTIFY subToolTipChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY iconNameChanged)

    Q_ENUMS(TrayStatus)

public:
    /**
     * System tray icon states.
     */
    enum TrayStatus {
        ActiveStatus=0,
        PassiveStatus
    };

    RemoteInstallerWidget(QObject *parent = nullptr);

    TrayStatus status() const;
    void changeTryIconState (QString mode);
    void setStatus(TrayStatus status);

    QString toolTip() const;
    void setToolTip(const QString &toolTip);

    QString subToolTip() const;
    void setSubToolTip(const QString &subToolTip);

    QString iconName() const;
    void setIconName(const QString &name);

    void isAlive();


public slots:
    
    void worker();
    void checkRemoteInstaller();

signals:
   
    void statusChanged();
    void toolTipChanged();
    void subToolTipChanged();
    void iconNameChanged();
 
private:

    QTimer *m_timer_run=nullptr;
    TrayStatus m_status = PassiveStatus;
    QString m_iconName = QStringLiteral("remote_installer_plugin");
    QString m_toolTip;
    QString m_subToolTip;
    QDir WATCH_DIR;
    QFile llxremote_sh;
    QFile llxremote_apt;
    QFile llxremote_deb;
    QFile llxremote_epi;
    QString llxremote_mode;
    bool is_working=false;
    QPointer<KNotification> notification;
    QFileSystemWatcher *watcher = nullptr;
    QString refPath="/tmp/.LLXRemoteInstallerClient";

    void cleanCache();

     
};

#endif // PLASMA_REMOTE_INSTALLER_WIDGET_H
