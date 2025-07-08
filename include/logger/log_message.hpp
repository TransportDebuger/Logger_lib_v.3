#pragma once

#include <chrono>
#include <string>
#include <thread>

#include "log_level.hpp"

namespace stc {

/**
 * @struct LogMessage
 * @brief Структурированное сообщение для логгирования.
 *
 * Содержит ключевые данные для форматирования и обработки лог-сообщения.
 */
struct LogMessage {
    LogLevel level;    ///< Уровень сообщения (debug, info, warning, error, fatal).
    std::string text;  ///< Текст сообщения.
    std::chrono::system_clock::time_point timestamp;  ///< Время формирования сообщения.
    std::thread::id thread_id;  ///< Идентификатор потока, из которого поступило сообщение.
    std::string component;  ///< Имя компонента или модуля (опционально).

    /**
     * @brief Конструктор сообщения.
     * @param lvl Уровень лога
     * @param msg Текст сообщения
     * @param comp Имя компонента (по умолчанию — пусто)
     */
    LogMessage(LogLevel lvl, const std::string& msg, const std::string& comp = "")
        : level(lvl),
          text(msg),
          timestamp(std::chrono::system_clock::now()),
          thread_id(std::this_thread::get_id()),
          component(comp) {}

    /// Конструктор по умолчанию
    LogMessage()
        : level(LogLevel::Info),
          timestamp(std::chrono::system_clock::now()),
          thread_id(std::this_thread::get_id()) {}
};

}  // namespace stc