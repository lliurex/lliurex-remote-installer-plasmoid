import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.1 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents3
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.plasma.private.remoteinstaller 1.0
// Item - the most basic plasmoid component, an empty container.
Item {

    id:remoteInstallerApplet
    
    RemoteInstallerWidget{
        id:remoteInstallerWidget

    }


    Plasmoid.status: {
        /* Warn! Enum types are accesed through ClassName not ObjectName */
        switch (remoteInstallerWidget.status){
            case RemoteInstallerWidget.ActiveStatus:
                return PlasmaCore.Types.ActiveStatus
            case RemoteInstallerWidget.PassiveStatus:
                return PlasmaCore.Types.PassiveStatus
           
        }
        return  PlasmaCore.Types.ActiveStatus
        
    }

    Plasmoid.switchWidth: units.gridUnit * 5
    Plasmoid.switchHeight: units.gridUnit * 5

    Plasmoid.icon:remoteInstallerWidget.iconName
    Plasmoid.toolTipMainText: remoteInstallerWidget.toolTip
    Plasmoid.toolTipSubText: remoteInstallerWidget.subToolTip

    Component.onCompleted: {
       plasmoid.removeAction("configure");
                
    }

   
    Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation
    Plasmoid.fullRepresentation: PlasmaComponents3.Page {
        implicitWidth: PlasmaCore.Units.gridUnit * 12
        implicitHeight: PlasmaCore.Units.gridUnit * 6

        PlasmaExtras.PlaceholderMessage {
            id:phMsg
            anchors.centerIn: parent
            width: parent.width - (PlasmaCore.Units.gridUnit * 4)
            iconName: Plasmoid.icon
            text:remoteInstallerWidget.subToolTip
        }
                
    }
    
 }  
