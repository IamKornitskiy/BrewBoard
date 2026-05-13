add_library(Settings
        AbstractSettingsManager.cpp AbstractSettingsManager.h
        AppSettingsManager.h
        AppSettingsManager.cpp
        #LoggerSettingsManager.h LoggerSettingsManager.cpp
)

target_include_directories(Settings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Settings PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Logger)