/**
 * @file log_level.cpp
 * @author Artem Ulyanov (https://github.com/TransportDebuger)
 * @date 2025-12-13
 * @version 1.2
 * 
 * @brief Реализация функций преобразования и фильтрации уровней логирования.
 */
#include "logger/log_level.hpp"

#include <algorithm>
#include <stdexcept>
#include <unordered_map>

namespace stc {

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Fatal:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

LogLevel stringToLogLevel(const std::string& level_str) {
    static const std::unordered_map<std::string, LogLevel> string_to_level = {
        {"DEBUG",    LogLevel::Debug},
        {"INFO",     LogLevel::Info},
        {"WARNING",  LogLevel::Warning},
        {"WARN",     LogLevel::Warning}, // Альтернативное название для WARNING
        {"ERROR",    LogLevel::Error},
        {"FATAL",    LogLevel::Fatal},
        {"CRITICAL", LogLevel::Fatal} // Альтернативное название для FATAL
    };
    
    std::string level_upper = level_str;
    std::transform(level_upper.begin(), level_upper.end(), level_upper.begin(), ::toupper);

    auto it = string_to_level.find(level_upper);
    if (it != string_to_level.end()) {
        return it->second;
    }

    throw std::invalid_argument("Unknown log level: " + level_str);
}

bool shouldLog(LogLevel message_level, LogLevel min_level) {
    return static_cast<int>(message_level) >= static_cast<int>(min_level);
}

}  // namespace stc