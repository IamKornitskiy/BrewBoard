add_library(Logger
    Logger.cpp
    Logger.h
    LoggerMacros.h
)

# Подключение директорий для заголовков
target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Подключение зависимостей
target_link_libraries(Logger PUBLIC Qt${QT_VERSION_MAJOR}::Core
                        Settings
)