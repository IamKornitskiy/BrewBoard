find_package(Qt6 REQUIRED COMPONENTS Core Test)

add_executable(test_app_settings
    tst_AppSettingsManager.cpp
    ../src/Settings/AppSettingsManager.cpp
    ../src/Settings/AbstractSettingsManager.cpp
)

target_include_directories(test_app_settings PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../src/Settings
    ../src/Logger
)

target_link_libraries(test_app_settings PRIVATE
    Qt6::Core
    Qt6::Test
    Logger
    Settings
)

add_test(NAME AppSettingsManager COMMAND test_app_settings)