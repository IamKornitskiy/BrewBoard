find_package(Qt6 REQUIRED COMPONENTS Core Test)

add_executable(test_app_settings
        tst_AppSettingsManager.cpp
        ../src/Settings/AppSettingsManager.cpp
        ../src/Settings/AbstractSettingsManager.cpp
)

target_include_directories(test_app_settings PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
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

# Тест UntappdApiClient
add_executable(test_untappd_api
        tst_UntappdApiClient.cpp
        ../src/Api/UntappdApiClient.cpp
        ../src/Models/Beer.cpp   # Beer нужен для парсинга
)

target_include_directories(test_untappd_api PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ../src/Api
        ../src/Models
        ../src/Logger
)

target_link_libraries(test_untappd_api PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Test
)

add_test(NAME UntappdApiClient COMMAND test_untappd_api)