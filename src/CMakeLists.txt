# Effect plugin
set(WAYMCA_TARGET kwin_effect_waymca)

add_library(${WAYMCA_TARGET} MODULE
    main.cpp
    waymca.cpp
    waymca.h
)

target_link_libraries(${WAYMCA_TARGET}
    KWin::kwin
    KF6::GlobalAccel
    Qt6::Core
    Qt6::DBus
    epoxy::epoxy
)

install(TARGETS ${WAYMCA_TARGET} DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins)

# Configuration module
set(WAYMCA_CONFIG_TARGET kwin_effect_waymca_config)

set(waymca_config_SOURCES waymca_config.cpp)
ki18n_wrap_ui(waymca_config_SOURCES waymca_config.ui)
kconfig_add_kcfg_files(waymca_config_SOURCES waymcaconfig.kcfgc)

add_library(${WAYMCA_CONFIG_TARGET} MODULE ${waymca_config_SOURCES})

target_link_libraries(${WAYMCA_CONFIG_TARGET}
    KWin::kwin
    Qt6::Widgets
    Qt6::Core
    Qt6::DBus
    KF6::ConfigCore
    KF6::ConfigWidgets
    KF6::CoreAddons
    KF6::I18n
    KF6::KCMUtils
)

install(TARGETS ${WAYMCA_CONFIG_TARGET} DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/configs)

message(STATUS "WayMCA: Building with configuration module")
