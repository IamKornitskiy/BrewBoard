#ifndef LOGGERMACROS_H
#define LOGGERMACROS_H

#include "Logger.h"

#define LOG_INFO(message) Logger::instance().log(Logger::Info, message, __FILE__, __LINE__)
#define LOG_WARNING(message) Logger::instance().log(Logger::Warning, message, __FILE__, __LINE__)
#define LOG_ERROR(message) Logger::instance().log(Logger::Error, message, __FILE__, __LINE__)
#define LOG_DEBUG(message) Logger::instance().log(Logger::Debug, message, __FILE__, __LINE__)

#endif // LOGGERMACROS_H