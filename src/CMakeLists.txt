cmake_minimum_required(VERSION 3.5)

project(sim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# --- 1. Konfiguracja Głównej Aplikacji (Qt) ---
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

add_executable(sim_app
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(sim_app PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Sql
)

# --- 2. Konfiguracja Walidatora (LibXml2) ---
# Próbujemy znaleźć bibliotekę LibXml2 do narzędzia walidacji
find_package(LibXml2)

if(LIBXML2_FOUND)
    add_executable(xml_validator ../tools/xml_validation.cpp)
    target_include_directories(xml_validator PRIVATE ${LIBXML2_INCLUDE_DIR})
    target_link_libraries(xml_validator PRIVATE ${LIBXML2_LIBRARIES})
    message(STATUS "Validation tool will be built.")
else()
    message(WARNING "LibXml2 not found. xml_validator will NOT be built. Install libxml2-dev.")
endif()

# --- Instalacja ---
include(GNUInstallDirs)
install(TARGETS sim_app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)