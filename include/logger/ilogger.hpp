#pragma once

#include "log_level.hpp"
#include <string>

namespace stc {

/**
 * @class ILogger
 * @brief Абстрактный интерфейс логгера.
 *
 * Определяет базовые методы для записи логов и управления уровнем логирования.
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /// Записать сообщение с указанным уровнем.
    virtual void log(LogLevel level, const std::string& message) = 0;
    /// Установить минимальный уровень логирования.
    virtual void setLevel(LogLevel min_level) = 0;
    /// Получить текущий минимальный уровень логирования.
    virtual LogLevel getLevel() const = 0;
    /// Проверить, разрешено ли логировать сообщение с данным уровнем.
    virtual bool isEnabled(LogLevel level) const = 0;

    // Удобные методы для разных уровней
    void debug(const std::string& msg) {
        if (isEnabled(LogLevel::Debug)) log(LogLevel::Debug, msg);
    }
    void info(const std::string& msg) {
        if (isEnabled(LogLevel::Info)) log(LogLevel::Info, msg);
    }
    void warning(const std::string& msg) {
        if (isEnabled(LogLevel::Warning)) log(LogLevel::Warning, msg);
    }
    void error(const std::string& msg) {
        if (isEnabled(LogLevel::Error)) log(LogLevel::Error, msg);
    }
    void fatal(const std::string& msg) {
        if (isEnabled(LogLevel::Fatal)) log(LogLevel::Fatal, msg);
    }
};

} // namespace stc