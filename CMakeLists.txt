cmake_minimum_required(VERSION 3.16)
project(chessgame VERSION 0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Create the executable with all source files
qt_add_executable(chessgame
    main.cpp
    mainwindow.cpp
    mainwindow.h
    chessboardwidget.cpp
    chessboardwidget.h
    plateau.cpp
    plateau.h
)

# Link Qt libraries (ONLY ONCE!)
target_link_libraries(chessgame PRIVATE Qt6::Core Qt6::Widgets)

# Windows specific: hide console window
if(WIN32)
    set_target_properties(chessgame PROPERTIES WIN32_EXECUTABLE TRUE)
endif()