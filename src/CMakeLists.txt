cmake_minimum_required(VERSION 3.5)

project(TODOApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    Task.cpp
    TaskManager.cpp
)

set(HEADERS
    mainwindow.h
    Task.h
    TaskManager.h
)

set(UIS
    mainwindow.ui
)

qt5_wrap_ui(UI_HEADERS ${UIS})

add_executable(TODOApp ${SOURCES} ${HEADERS} ${UI_HEADERS})

target_link_libraries(TODOApp Qt5::Widgets)