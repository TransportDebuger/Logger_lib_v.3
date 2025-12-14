#pragma once

#include "ilogger.hpp"

namespace stc {

class BaseLogger : public ILogger {
 public:
  BaseLogger() = default;
  ~BaseLogger() override = default;

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
   * */
  void warning(const std::string& msg) {
    if (isEnabled(LogLevel::Warning)) log(LogLevel::Warning, msg);
  }

  /**
   * @brief Записать сообщение уровня Error.
   * @param msg Текст сообщения.
   *
   * Используется для вывода предупреждений.
   * */
  void error(const std::string& msg) {
    if (isEnabled(LogLevel::Error)) log(LogLevel::Error, msg);
  }

  /**
   * @brief Записать сообщение уровня Fatal.
   * @param msg Текст сообщения.
   *
   * Используется для вывода предупреждений.
   * */
  void fatal(const std::string& msg) {
    if (isEnabled(LogLevel::Fatal)) log(LogLevel::Fatal, msg);
  }
};

}  // namespace stc