cmake_minimum_required(VERSION 3.16)

project(waymca)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

find_package(KF6 REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    I18n
)

find_package(epoxy REQUIRED)
find_package(KWinEffects REQUIRED COMPONENTS kwineffects)
find_package(KWin REQUIRED COMPONENTS kwineffects)

set(waymca_SOURCES
    src/waymca.cpp
)

kconfig_add_kcfg_files(waymca_SOURCES src/waymca.kcfg)

add_library(waymca MODULE ${waymca_SOURCES})

target_link_libraries(waymca
    Qt6::Core
    Qt6::Gui
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::CoreAddons
    KF6::I18n
    epoxy::epoxy
    kwineffects
)

install(TARGETS waymca DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/plugins)
install(FILES metadata.json DESTINATION ${KDE_INSTALL_DATADIR}/kwin/effects/waymca)
install(FILES src/waymca.frag DESTINATION ${KDE_INSTALL_DATADIR}/kwin/effects/waymca)

# Configuration module
set(waymca_config_SOURCES
    src/waymca_config.cpp
)

ki18n_wrap_ui(waymca_config_SOURCES src/waymca_config.ui)
kconfig_add_kcfg_files(waymca_config_SOURCES src/waymca.kcfg)

add_library(waymca_config MODULE ${waymca_config_SOURCES})

target_link_libraries(waymca_config
    Qt6::Widgets
    KF6::ConfigCore
    KF6::ConfigWidgets
    KF6::CoreAddons
    KF6::I18n
)

install(TARGETS waymca_config DESTINATION ${KDE_INSTALL_PLUGINDIR}/kwin/effects/configs)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
