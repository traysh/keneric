cmake_minimum_required(VERSION 3.29)

project(keneric)

set(QT_MIN_VERSION "6.5.0")
set(KF6_MIN_VERSION "5.240.0")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} )

include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED NO_MODULE COMPONENTS Gui)
find_package(KF6 REQUIRED COMPONENTS KIO)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )


kcoreaddons_add_plugin(keneric INSTALL_NAMESPACE "kf${QT_MAJOR_VERSION}/thumbcreator")

target_sources(keneric PRIVATE
    keneric.cpp
)

target_link_libraries(keneric Qt6::Gui KF6::KIOWidgets)

install(PROGRAMS scripts/kenericProcess DESTINATION bin)
