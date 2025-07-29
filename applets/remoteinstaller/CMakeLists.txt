
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.remoteinstaller)


#######################################################################################
# Notes Library
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.remoteinstaller")

set(remoteinstallerwidget_SRCS
    plugin/plugin.cpp
    plugin/RemoteInstallerWidget.cpp
   
)

add_library(remoteinstallerplugin SHARED ${remoteinstallerwidget_SRCS})

target_link_libraries(remoteinstallerplugin
                      Qt5::Quick
                      Qt5::Core
                      Qt5::Widgets
                      KF5::CoreAddons
                      KF5::I18n
                      KF5::Notifications
                      KF5::KIOWidgets
                      )

install(FILES plugin/qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/remoteinstaller)
install(TARGETS remoteinstallerplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/remoteinstaller)
install(FILES "icons/remote_installer_plugin.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "knotifications5/remoteinstaller.notifyrc"
       DESTINATION ${DATAROOTDIR}/usr/share/knotifications5/)
