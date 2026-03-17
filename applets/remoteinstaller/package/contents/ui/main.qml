import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.plasma.plasmoid
import org.kde.plasma.core as PlasmaCore
import org.kde.plasma.components as PlasmaComponents3
import org.kde.plasma.extras as PlasmaExtras
import org.kde.kirigami as Kirigami

import org.kde.plasma.private.remoteinstaller
// Item - the most basic plasmoid component, an empty container.

PlasmoidItem {

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

    switchWidth: Kirigami.Units.gridUnit * 5
    switchHeight: Kirigami.Units.gridUnit * 5

    Plasmoid.icon:remoteInstallerWidget.iconName
    toolTipMainText: remoteInstallerWidget.toolTip
    toolTipSubText: remoteInstallerWidget.subToolTip

    fullRepresentation: PlasmaComponents3.Page {
        implicitWidth: Kirigami.Units.gridUnit * 12
        implicitHeight: Kirigami.Units.gridUnit * 6

        PlasmaExtras.PlaceholderMessage {
            id:phMsg
            anchors.centerIn: parent
            width: parent.width - (Kirigami.Units.gridUnit * 4)
            iconName: Plasmoid.icon
            text:remoteInstallerWidget.subToolTip
        }
                
    }
    
 }  
