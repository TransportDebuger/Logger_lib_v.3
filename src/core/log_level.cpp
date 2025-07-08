#include "logger/log_level.hpp"
#include <algorithm>
#include <stdexcept>

namespace stc {

std::string logLevelToString(LogLevel level) {
    switch(level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Fatal: return "FATAL";
        default: return "UNKNOWN";
    }
}

LogLevel stringToLogLevel(const std::string& level_str) {
    std::string level_upper = level_str;
    std::transform(level_upper.begin(), level_upper.end(), level_upper.begin(), ::toupper);

    if (level_upper == "DEBUG") return LogLevel::Debug;
    if (level_upper == "INFO") return LogLevel::Info;
    if (level_upper == "WARNING" || level_upper == "WARN") return LogLevel::Warning;
    if (level_upper == "ERROR") return LogLevel::Error;
    if (level_upper == "FATAL" || level_upper == "CRITICAL") return LogLevel::Fatal;

    throw std::invalid_argument("Unknown log level: " + level_str);
}

bool shouldLog(LogLevel message_level, LogLevel min_level) {
    return static_cast<int>(message_level) >= static_cast<int>(min_level);
}

} // namespace stc