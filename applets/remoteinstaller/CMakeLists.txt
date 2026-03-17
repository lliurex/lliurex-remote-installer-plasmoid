
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.remoteinstaller)


#######################################################################################
# Notes Library
set(PLUGIN_TARGET "remoteinstallerplugin")
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.remoteinstaller")
ecm_add_qml_module(${PLUGIN_TARGET} URI org.kde.plasma.private.remoteinstaller VERSION 2.0)

target_sources(${PLUGIN_TARGET} PRIVATE
    plugin/plugin.cpp
    plugin/RemoteInstallerWidget.cpp
    plugin/plugin.h
    plugin/RemoteInstallerWidget.h

)


target_link_libraries(remoteinstallerplugin PRIVATE
                      Qt6::Quick
                      Qt6::Core
                      Qt6::Widgets
                      Qt6::Concurrent
                      KF6::CoreAddons
                      KF6::I18n
                      KF6::Notifications
                      KF6::KIOWidgets

                      )

target_include_directories(${PLUGIN_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin)

ecm_finalize_qml_module(${PLUGIN_TARGET})

install(TARGETS ${PLUGIN_TARGET} DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/private/remoteinstaller)

install(FILES "icons/remote_installer_plugin.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/remote_installer_plugin_app.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "knotifications6/remoteinstaller.notifyrc"
        DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
