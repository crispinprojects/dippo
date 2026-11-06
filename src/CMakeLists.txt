cmake_minimum_required(VERSION 3.16)
project(DippoQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dippo-qt
    main.cpp
    mainwindow.cpp
    mainwindow.h
    sanitizer.cpp

    sanitizer.cpp
    sanitizer.h
    synthesizer.cpp
    synthesizer.h
    dictionary.cpp
    dictionary.h
    diphone.cpp
    diphone.h
)

target_link_libraries(dippo-qt PRIVATE Qt6::Widgets)
