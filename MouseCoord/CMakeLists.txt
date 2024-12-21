cmake_minimum_required(VERSION 3.10)

project(MouseCoord
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(MouseCoord main.cpp mc.h mc.cpp globalhook.hpp)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

target_link_libraries(MouseCoord
                        PRIVATE
                        Qt6::Core 
                        Qt6::Widgets
                        Qt6::Gui
                        )
message(STATUS "QT_VERSION: -${QT_VERSION} -Qt${QT_VERSION_MAJOR}")