cmake_minimum_required(VERSION 3.16)
project(BrewBoard VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Qml Network)

add_subdirectory(src/Settings)
add_subdirectory(src/Logger)
add_subdirectory(src/Models)
add_subdirectory(src/Api)

add_executable(BrewBoard
        src/main.cpp
        qml/qml.qrc
)

target_link_libraries(BrewBoard
        Settings
        Logger
        Models
        Api
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Qt6::Network
)

set_target_properties(BrewBoard PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
)

option(BUILD_TESTS "Build unit tests" ON)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

install(TARGETS BrewBoard DESTINATION bin)

