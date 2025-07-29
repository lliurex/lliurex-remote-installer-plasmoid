#include "RemoteInstallerWidget.h"
#include <KLocalizedString>
#include <KFormat>
#include <KNotification>
#include <KRun>
#include <QTimer>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileSystemWatcher>
#include <QTime>
#include <QDate>



RemoteInstallerWidget::RemoteInstallerWidget(QObject *parent)
    : QObject(parent)
    , m_timer_run(new QTimer(this))
    
{
    
	cleanCache();
    WATCH_DIR=refPath;
    QString sh_token_path=WATCH_DIR.absoluteFilePath("llxremote_sh_token");
    llxremote_sh.setFileName(sh_token_path);
    QString apt_token_path=WATCH_DIR.absoluteFilePath("llxremote_apt_token");
    llxremote_apt.setFileName(apt_token_path);
    QString deb_token_path=WATCH_DIR.absoluteFilePath("llxremote_deb_token");
    llxremote_deb.setFileName(deb_token_path);
    QString epi_token_path=WATCH_DIR.absoluteFilePath("llxremote_epi_token");
    llxremote_epi.setFileName(epi_token_path);


    QString titleTail=i18n("Is not running");
    setSubToolTip(titleTail);
    initWatcher();
   
}

void RemoteInstallerWidget::cleanCache(){

    QFile CURRENT_VERSION_TOKEN;
    QDir cacheDir("/home/"+user+"/.cache/plasmashell/qmlcache");
    QString currentVersion="";
    bool clear=false;

    CURRENT_VERSION_TOKEN.setFileName("/home/"+user+"/.config/remote-installer-widget.conf");
    QString installedVersion=getInstalledVersion();

    if (!CURRENT_VERSION_TOKEN.exists()){
        if (CURRENT_VERSION_TOKEN.open(QIODevice::WriteOnly)){
            QTextStream data(&CURRENT_VERSION_TOKEN);
            data<<installedVersion;
            CURRENT_VERSION_TOKEN.close();
            clear=true;
        }
    }else{
        if (CURRENT_VERSION_TOKEN.open(QIODevice::ReadOnly)){
            QTextStream content(&CURRENT_VERSION_TOKEN);
            currentVersion=content.readLine();
            CURRENT_VERSION_TOKEN.close();
        }

        if (currentVersion!=installedVersion){
            if (CURRENT_VERSION_TOKEN.open(QIODevice::WriteOnly)){
                QTextStream data(&CURRENT_VERSION_TOKEN);
                data<<installedVersion;
                CURRENT_VERSION_TOKEN.close();
                clear=true;
            }
        }
    } 
    if (clear){
        if (cacheDir.exists()){
            cacheDir.removeRecursively();
        }
    }   

}

QString RemoteInstallerWidget::getInstalledVersion(){

    QFile INSTALLED_VERSION_TOKEN;
    QString installedVersion="";
    
    INSTALLED_VERSION_TOKEN.setFileName("/var/lib/lliurex-remote-installer-indicator/version");

    if (INSTALLED_VERSION_TOKEN.exists()){
        if (INSTALLED_VERSION_TOKEN.open(QIODevice::ReadOnly)){
            QTextStream content(&INSTALLED_VERSION_TOKEN);
            installedVersion=content.readLine();
            INSTALLED_VERSION_TOKEN.close();
        }
    }
    return installedVersion;

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
	watcher=new QFileSystemWatcher(this);
	connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(worker()));
	watcher->addPath(refPath);
	if (initWorker){
		worker();
	}

}


void RemoteInstallerWidget::worker(){

    if (!is_working){
        if (RemoteInstallerWidget::llxremote_sh.exists() || RemoteInstallerWidget::llxremote_apt.exists() || RemoteInstallerWidget::llxremote_deb.exists() || RemoteInstallerWidget::llxremote_epi.exists()) {
            isAlive();
        }else{
        	setStatus(PassiveStatus);
        }
   	}	  	

}    


void RemoteInstallerWidget::isAlive(){

    is_working=true;
   	llxremote_mode="";

	if (RemoteInstallerWidget::llxremote_sh.exists()){
		llxremote_mode="sh";

	}else{
		if (RemoteInstallerWidget::llxremote_apt.exists()){
			llxremote_mode="apt";
		}else{
			if (RemoteInstallerWidget::llxremote_deb.exists()){
				llxremote_mode="deb";
			}else{
				if (RemoteInstallerWidget::llxremote_epi.exists()){
					llxremote_mode="epi"
				}
			}
		}
	}		

	changeTryIconState(llxremote_mode);
    
    connect(m_timer_run, &QTimer::timeout, this, &RemoteInstallerWidget::checkRemoteInstaller);
    m_timer_run->start(5000);
    checkRemoteInstaller();

}

void RemoteInstallerWidget::checkRemoteInstaller(){

	bool stop_widget=false;
	
	if (llxremote_mode=="sh"){
		if (!RemoteInstallerWidget::llxremote_sh.exists()){
				stop_widget=true;
	    }    	

	}else{
		if (llxremote_mode=="apt"){
			if (!RemoteInstallerWidget::llxremote_apt.exists()){
				stop_widget=true;
			}	

		}else{
			if (!RemoteInstallerWidget::llxremote_deb.exists()){
				stop_widget=true;
			}else{
				if (!RemoteInstallerWidget::llxremote_epi.exists()){
					stop_widget=true;
				}
			}
	    }
	 } 

	 if (stop_widget){
	 	m_timer_run->stop();
	    is_working=false;
	    QDate currentDate=QDate.currentDate();
	    QString lastDay=currentDate.toString(Qt::ISODate);
	    QTime currentTime=QTime.currentTime();
	    QString lastTime=curretTime.toString(Qt::ISODate);
	    QString titleTail=i18n("Last execution: ")+lastDay+" - "+lastTime;
	    setSubToolTip(titleTail);
	    setStatus(PassiveStatus);

	 }      	


}	


RemoteInstallerWidget::TrayStatus RemoteInstallerWidget::status() const
{
    return m_status;
}


void RemoteInstallerWidget::changeTryIconState(QString mode){

    const QString tooltip(i18n("LliureX Remote Installer"));
    setStatus(NeedsAttentionStatus);
    setIconName("remote_installer_plugin");
    setToolTip(tooltip);
    QString title(i18n("Is running: "));
     
    bool show_notification=false;

	if (mode=="sh"){
		QString sh_label(i18n("Executable Mode"));
		const QString subtooltip(title+sh_label);
		show_notification=true;
	       
	}else{
	  	if (mode=="apt"){
			QString apt_label(i18n("Apt Mode"));
	   		const QString subtooltip(title+apt_label);
	   		show_notification=true;
	   	}else{
	   		if (mode=="deb"){
				QString deb_label(i18n("Deb Mode"));
	  			const QString subtooltip(title+deb_label);
	  			show_notification=true;
 	        }else{
	        	if (mode=="epi"){
	        		QString deb_label(i18n("EPI Mode"));
	  				const QString subtooltip(title+deb_label);
	  				show_notification=true;
	        	}
	        }
	   	}	
	}

	if (show_notification){
		setSubToolTip(subtooltip);
        notification = KNotification::event(QStringLiteral("Run"), subtooltip, {}, "lliurex-remote-installer-indicator", nullptr, KNotification::CloseOnTimeout , QStringLiteral("llxremoteinstallernotifier"));
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