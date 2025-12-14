/**
 * @file log_level.hpp
 * @author Artem Ulyanov (https://github.com/TransportDebuger)
 * @date 2025-12-13
 * @version 1.2
 *
 * @brief Определяет уровни логирования и вспомогательные функции для их
 * преобразования и фильтрации.
 */

#pragma once

#include <string>

namespace stc {

/**
 * @ingroup Core
 * @enum LogLevel
 * @brief Уровни важности сообщений логирования.
 */
enum class LogLevel : int {
  Debug = 0,  ///< Детальные отладочные сообщения (наиболее низкий приоритет)
  Info = 1,  ///< Информационные события, отражающие ход работы системы
  Warning = 2,  ///< Предупреждения: непредусмотренные, но не критичные ситуации
  Error = 3,  ///< Ошибки: сбой операции, но работа приложения продолжается
  Fatal = 4  ///< Критические ошибки: возможно, аварийное завершение (наиболее
             ///< высокий приоритет)
};

/**
 * @ingroup Core
 * @brief Преобразует уровень логирования в строковое представление.
 *
 * @param level Значение типа @ref stc::LogLevel
 *
 * @return Строковое представление уровня: "DEBUG", "INFO", "WARNING", "ERROR",
 * "FATAL". Для неизвестных значений возвращается "UNKNOWN".
 *
 * @exception noexcept Гарантированно не выбрасывает исключения.
 *
 * @note Всегда возвращает строку в верхнем регистре.
 */
std::string logLevelToString(LogLevel level);

/**
 * @ingroup Core
 * @brief  Преобразует строку в уровень логирования (регистронезависимо).
 *
 * @param level_str Строковое представление уровня: "debug", "INFO", "warn" и
 * т.п. Регистр символов игнорируется.
 *
 * @return Соответствующее значение типа `stc::LogLevel`.
 *
 * @exception std::invalid_argument Если строка не соответствует ни одному
 *                                  поддерживаемому уровню или синониму.
 *                                  Сообщение: "Unknown log level: {level_str}".
 * @ingroup Core
 * @warning Пробелы в начале/конце не обрезаются — должны быть обработаны
 * заранее в вызывающем коде.
 */
LogLevel stringToLogLevel(const std::string& level_str);

/**
 * @brief Проверяет, должно ли сообщение быть залогировано.
 *
 * @param message_level Уровень логируемого сообщения.
 * @param min_level     Минимальный уровень логгируемых сообщений.
 * @return true, если `message_level >= min_level`, иначе false.
 *
 * @exception noexcept Гарантирует отсутствие исключений и побочных эффектов.
 * @ingroup Core
 */
bool shouldLog(LogLevel message_level, LogLevel min_level);

}  // namespace stc