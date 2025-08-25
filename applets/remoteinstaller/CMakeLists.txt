
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.remoteinstaller)


#######################################################################################
# Notes Library
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.remoteinstaller")
ecm_add_qml_module(remoteinstallerplugin URI org.kde.plasma.private.remoteinstaller)

target_sources(remoteinstallerplugin PRIVATE
    plugin/plugin.cpp
    plugin/RemoteInstallerWidget.cpp

)


target_link_libraries(remoteinstallerplugin PRIVATE
                      Qt6::Quick
                      Qt6::Core
                      Qt6::Widgets
                      KF6::CoreAddons
                      KF6::I18n
                      KF6::Notifications
                      KF6::KIOWidgets

                      )

ecm_finalize_qml_module(remoteinstallerplugin)

install(FILES plugin/qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/remoteinstaller)
install(TARGETS remoteinstallerplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/remoteinstaller)

install(FILES "icons/remote_installer_plugin.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/remote_installer_plugin_app.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "knotifications6/remoteinstaller.notifyrc"
        DESTINATION ${DATAROOTDIR}/usr/share/knotifications6/)   
