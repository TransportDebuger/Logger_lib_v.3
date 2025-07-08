#pragma once

#include <string>

namespace stc {

/**
 * @enum LogLevel
 * @brief Уровни важности сообщений логгирования.
 *
 * @details Уровни упорядочены по возрастанию важности:
 *          Debug < Info < Warning < Error < Fatal
 */
enum class LogLevel : int {
    Debug = 0,    ///< Отладочная информация
    Info = 1,     ///< Информационные сообщения
    Warning = 2,  ///< Предупреждения
    Error = 3,    ///< Ошибки
    Fatal = 4     ///< Критические ошибки
};

/**
 * @brief Преобразование уровня логгирования в строку.
 * @param level Уровень логгирования
 * @return Строковое представление уровня
 *
 * @code{.cpp}
 * std::string level_str = logLevelToString(LogLevel::Info);
 * // level_str == "INFO"
 * @endcode
 */
std::string logLevelToString(LogLevel level);

/**
 * @brief Преобразование строки в уровень логгирования.
 * @param level_str Строковое представление уровня (регистр не важен)
 * @return Уровень логгирования
 * @throws std::invalid_argument при неизвестном уровне
 *
 * @code{.cpp}
 * LogLevel level = stringToLogLevel("info");  // == LogLevel::Info
 * LogLevel level2 = stringToLogLevel("ERROR"); // == LogLevel::Error
 * @endcode
 */
LogLevel stringToLogLevel(const std::string& level_str);

/**
 * @brief Проверка, должно ли сообщение быть записано.
 * @param message_level Уровень сообщения
 * @param min_level Минимальный уровень для записи
 * @return true, если сообщение должно быть записано
 *
 * @code{.cpp}
 * bool should_log = shouldLog(LogLevel::Warning, LogLevel::Info);
 * // should_log == true (Warning >= Info)
 * @endcode
 */
bool shouldLog(LogLevel message_level, LogLevel min_level);

}  // namespace stc