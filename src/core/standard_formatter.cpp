#include "logger/standard_formatter.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

#include "logger/log_level.hpp"

namespace stc {

StandardFormatter::StandardFormatter()
    : pattern_("[{timestamp}] [{level}] [{component}] {message}") {}

StandardFormatter::StandardFormatter(const std::string& pattern) : pattern_(pattern) {}

std::string StandardFormatter::format(const LogMessage& message) {
    return replacePlaceholders(pattern_, message);
}

void StandardFormatter::setPattern(const std::string& pattern) {
    pattern_ = pattern;
}

const std::string& StandardFormatter::getPattern() const {
    return pattern_;
}

std::string StandardFormatter::formatTimestamp(
    const std::chrono::system_clock::time_point& timestamp) const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(timestamp.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}

std::string StandardFormatter::formatThreadId(const std::thread::id& thread_id) const {
    std::stringstream ss;
    ss << thread_id;
    return ss.str();
}

std::string StandardFormatter::replacePlaceholders(const std::string& pattern,
                                                   const LogMessage& message) const {
    std::string result = pattern;

    // Заменяем плейсхолдеры
    size_t pos = 0;
    while ((pos = result.find("{timestamp}", pos)) != std::string::npos) {
        result.replace(pos, 11, formatTimestamp(message.timestamp));
        pos += 11;
    }

    pos = 0;
    while ((pos = result.find("{level}", pos)) != std::string::npos) {
        result.replace(pos, 7, logLevelToString(message.level));
        pos += 7;
    }

    pos = 0;
    while ((pos = result.find("{component}", pos)) != std::string::npos) {
        std::string component = message.component.empty() ? "MAIN" : message.component;
        result.replace(pos, 11, component);
        pos += component.length();
    }

    pos = 0;
    while ((pos = result.find("{message}", pos)) != std::string::npos) {
        result.replace(pos, 9, message.text);
        pos += message.text.length();
    }

    pos = 0;
    while ((pos = result.find("{thread_id}", pos)) != std::string::npos) {
        result.replace(pos, 11, formatThreadId(message.thread_id));
        pos += 11;
    }

    return result;
}

}  // namespace stc