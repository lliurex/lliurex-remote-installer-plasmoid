#include "RemoteInstallerWidget.h"
#include <KLocalizedString>
#include <KNotification>

#include <QTimer>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileSystemWatcher>
#include <QTime>
#include <QDate>

RemoteInstallerWidget::RemoteInstallerWidget(QObject *parent)
    : QObject(parent)
    ,m_watcher_timer(new QTimer(this))
    
{
    
    WATCH_DIR.setPath(refPath);
    QString sh_token_path=WATCH_DIR.absoluteFilePath("llxremote_sh_token");
    llxremote_sh.setFileName(sh_token_path);
    QString apt_token_path=WATCH_DIR.absoluteFilePath("llxremote_apt_token");
    llxremote_apt.setFileName(apt_token_path);
    QString deb_token_path=WATCH_DIR.absoluteFilePath("llxremote_deb_token");
    llxremote_deb.setFileName(deb_token_path);
    QString epi_token_path=WATCH_DIR.absoluteFilePath("llxremote_epi_token");
    llxremote_epi.setFileName(epi_token_path);

    notificationBody=i18n("Is not running");
    setSubToolTip(notificationBody);

    m_watcher_timer->setSingleShot(true);
    m_watcher_timer->setInterval(500);
    connect(m_watcher_timer, &QTimer::timeout, this, &RemoteInstallerWidget::worker);
    initWatcher();
   
}

void RemoteInstallerWidget::initWatcher(){

    QDir TARGET_DIR(refPath);
    bool initWorker=false;

	if (!TARGET_DIR.exists()){
		QDir basePath("/tmp/");
		basePath.mkdir(".LLXRemoteInstallerClient");
	}else{
		initWorker=true;
	}

    if (!watcher) {
        watcher = new QFileSystemWatcher(this);
        connect(watcher, &QFileSystemWatcher::directoryChanged, [this](const QString &path){
            Q_UNUSED(path);
            m_watcher_timer->start(); 
        });
    }
    if (watcher->directories().isEmpty()) {
    	watcher->addPath(refPath);
    }

	if (initWorker){
		worker();
	}

}


void RemoteInstallerWidget::worker(){

	if (RemoteInstallerWidget::llxremote_sh.exists() || RemoteInstallerWidget::llxremote_apt.exists() || RemoteInstallerWidget::llxremote_deb.exists() || RemoteInstallerWidget::llxremote_epi.exists()) {
		isAlive();
	}else{
		if (is_working){
			showEndNotification();
		}else{
			setStatus(PassiveStatus);
		}
	}

}    

void RemoteInstallerWidget::isAlive(){
  
   	llxremote_mode="";

	if (RemoteInstallerWidget::llxremote_sh.exists()){
		llxremote_mode="sh";
	}else if (RemoteInstallerWidget::llxremote_apt.exists()){
		llxremote_mode="apt";
	}else if (RemoteInstallerWidget::llxremote_deb.exists()){
		llxremote_mode="deb";
	}else if (RemoteInstallerWidget::llxremote_epi.exists()){
		llxremote_mode="epi";
	}

	if (!llxremote_mode.isEmpty()){
		changeTryIconState(llxremote_mode);
		if (!is_working){
    		is_working=true;
    	}
    }else{
    	if (is_working){
    		showEndNotification();
    	}else{
    		setStatus(PassiveStatus);
    	}

    }	

}

void RemoteInstallerWidget::showEndNotification(){

    is_working=false;
    m_watcher_timer->stop();
    QDate currentDate=QDate::currentDate();
    QString lastDay=currentDate.toString(Qt::ISODate);
    QTime currentTime=QTime::currentTime();
    QString lastTime=currentTime.toString(Qt::ISODate);
    notificationBody=i18n("Last execution: ")+lastDay+" - "+lastTime+"\n"+i18n("Last action executed: ")+notificationAction;
    QString endBody=i18n("Has finished executing the actions");
    notification = KNotification::event(QStringLiteral("Run"), endBody, {}, "remote_installer_plugin", nullptr, KNotification::CloseOnTimeout , QStringLiteral("remoteinstaller"));
    setSubToolTip(notificationBody);
    setStatus(PassiveStatus);
}	


RemoteInstallerWidget::TrayStatus RemoteInstallerWidget::status() const
{
    return m_status;
}


void RemoteInstallerWidget::changeTryIconState(QString mode){

    const QString tooltip(i18n("LliureX Remote Installer"));
    setStatus(ActiveStatus);
    setIconName("remote_installer_plugin");
    setToolTip(tooltip);
    QString title(i18n("Executing action type: "));
     
    bool show_notification=false;


	switch(mode_options.indexOf(mode)){
		case 0:{
			notificationAction=i18n("EXECUTABLES");
			show_notification=true;
	    	break;
	    }
		case 1:{
			notificationAction=i18n("APT");
	   		show_notification=true;
	   		break;
	   	}
	   	case 2:{
			notificationAction=i18n("DEB");
	  		show_notification=true;
 	        break;
 	    }
	    case 3:{
	        notificationAction=i18n("ZMD");
	  		show_notification=true;
	        break;
	    }
	}	

	if (show_notification){
		setSubToolTip(title+notificationAction);
        notification = KNotification::event(QStringLiteral("Run"), title+notificationAction, {}, "remote_installer_plugin", nullptr, KNotification::CloseOnTimeout , QStringLiteral("remoteinstaller"));
	}
      
}


void RemoteInstallerWidget::setStatus(RemoteInstallerWidget::TrayStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

QString RemoteInstallerWidget::iconName() const
{
    return m_iconName;
}

void RemoteInstallerWidget::setIconName(const QString &name)
{
    if (m_iconName != name) {
        m_iconName = name;
        emit iconNameChanged();
    }
}

QString RemoteInstallerWidget::toolTip() const
{
    return m_toolTip;
}

void RemoteInstallerWidget::setToolTip(const QString &toolTip)
{
    if (m_toolTip != toolTip) {
        m_toolTip = toolTip;
        emit toolTipChanged();
    }
}

QString RemoteInstallerWidget::subToolTip() const
{
    return m_subToolTip;
}

void RemoteInstallerWidget::setSubToolTip(const QString &subToolTip)
{
    if (m_subToolTip != subToolTip) {
        m_subToolTip = subToolTip;
        emit subToolTipChanged();
    }
}
