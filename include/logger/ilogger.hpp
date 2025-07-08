#pragma once

#include "log_level.hpp"
#include <string>

namespace stc {

/**
 * @class ILogger
 * @brief Абстрактный интерфейс логгера.
 *
 * @details Определяет базовые методы для записи логов и управления уровнем логирования.
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
     * @brief Записать сообщение c указанным уровнем.
     * @param level Уровень сообщения.
     * @param msg Текст сообщения.
     *
     * @details Используется записи сообщения в лог.
     */
    virtual void log(LogLevel level, const std::string& msg) = 0;
    
    /**
     * @brief Установка уровня логгирования.
     * @param min_level Уровень сообщения.
     *
     * @details Устанавливает минимальный уровень логгирования сообщения.
     */
    virtual void setLevel(LogLevel min_level) = 0;
    
    /**
     * @brief Записать сообщение c указанным уровнем.
     * @return Возвращает минимальный уровень логгирования (LogLevel).
     *
     * @details Используется записи сообщения в лог.
     */
    virtual LogLevel getLevel() const = 0;

    /**
     * @brief Прверка соотвествия минимально допустимого уровня логиирования.
     * @param level Уровень логирования.
     * @retval true Если уровень логгирования установлен ниже или равным 
     *         уровню переданного сообщения.
     * @retval false Если уровень логгирования установлен выше чем уровень
     *         передаваемого сообщения. 
     *
     * @details Используется записи сообщения в лог.
     */
    virtual bool isEnabled(LogLevel level) const = 0;

    /**
     * @brief Записать сообщение уровня Debug.
     * @param msg Текст сообщения.
     *
     * @details Используется для вывода отладочной информации.
     */
    void debug(const std::string& msg) {
        if (isEnabled(LogLevel::Debug)) log(LogLevel::Debug, msg);
    }

    /**
     * @brief Записать сообщение уровня Info.
     * @param msg Текст сообщения.
     *
     * Используется для вывода информационных сообщений.
     */
    void info(const std::string& msg) {
        if (isEnabled(LogLevel::Info)) log(LogLevel::Info, msg);
    }

    /**
     * @brief Записать сообщение уровня Warning.
     * @param msg Текст сообщения.
     *
     * Используется для вывода предупреждений.
     */
    void warning(const std::string& msg) {
        if (isEnabled(LogLevel::Warning)) log(LogLevel::Warning, msg);
    }

    /**
     * @brief Записать сообщение уровня Error.
     * @param msg Текст сообщения.
     *
     * Используется для вывода ошибок.
     */
    void error(const std::string& msg) {
        if (isEnabled(LogLevel::Error)) log(LogLevel::Error, msg);
    }

    /**
     * @brief Записать сообщение уровня Fatal.
     * @param msg Текст сообщения.
     *
     * Используется для вывода критических ошибок.
     */
    void fatal(const std::string& msg) {
        if (isEnabled(LogLevel::Fatal)) log(LogLevel::Fatal, msg);
    }
};

} // namespace stc