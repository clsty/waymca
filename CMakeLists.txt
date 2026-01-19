cmake_minimum_required(VERSION 3.16)

project(waymca)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_MIN_VERSION "6.6.0")
set(KF6_MIN_VERSION "6.0.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Gui
    Widgets
)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    GlobalAccel
    WindowSystem
)

# Optional components for config module
find_package(KF6Config ${KF6_MIN_VERSION} REQUIRED)
find_package(KF6ConfigWidgets ${KF6_MIN_VERSION} REQUIRED)
find_package(KF6CoreAddons ${KF6_MIN_VERSION} REQUIRED)
find_package(KF6I18n ${KF6_MIN_VERSION} REQUIRED)
find_package(KF6KCMUtils ${KF6_MIN_VERSION} REQUIRED)

find_package(KWin REQUIRED COMPONENTS kwineffects)
find_package(epoxy REQUIRED)
find_package(KWinDBusInterface CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(data)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
