#pragma once

#include <iomanip>
#include <sstream>
#include <string>

#include "iformatter.hpp"

namespace stc {

/**
 * @brief Стандартный форматтер для сообщений логгирования.
 *
 * Форматирует сообщения в виде: [timestamp] [level] [component] message
 * или по настраиваемому шаблону.
 */
class StandardFormatter : public IFormatter {
public:
    /**
     * @brief Конструктор по умолчанию.
     *
     * Использует стандартный шаблон форматирования:
     * "[{timestamp}] [{level}] [{component}] {message}"
     */
    StandardFormatter();

    /**
     * @brief Конструктор с настраиваемым шаблоном.
     * @param pattern Шаблон форматирования с плейсхолдерами:
     *   - {timestamp} - время сообщения
     *   - {level} - уровень логгирования
     *   - {component} - имя компонента
     *   - {message} - текст сообщения
     *   - {thread_id} - идентификатор потока
     */
    explicit StandardFormatter(const std::string& pattern);

    /**
     * @brief Форматирует сообщение логгирования в строку.
     * @param message Структурированное сообщение для форматирования.
     * @return Отформатированная строка.
     */
    std::string format(const LogMessage& message) override;

    /**
     * @brief Устанавливает новый шаблон форматирования.
     * @param pattern Новый шаблон форматирования.
     */
    void setPattern(const std::string& pattern);

    /**
     * @brief Получает текущий шаблон форматирования.
     * @return Текущий шаблон.
     */
    const std::string& getPattern() const;

private:
    /// Шаблон форматирования
    std::string pattern_;

    /**
     * @brief Преобразует timestamp в строку.
     * @param timestamp Время для преобразования.
     * @return Строковое представление времени.
     */
    std::string formatTimestamp(const std::chrono::system_clock::time_point& timestamp) const;

    /**
     * @brief Преобразует thread_id в строку.
     * @param thread_id Идентификатор потока.
     * @return Строковое представление ID потока.
     */
    std::string formatThreadId(const std::thread::id& thread_id) const;

    /**
     * @brief Заменяет плейсхолдеры в шаблоне на реальные значения.
     * @param pattern Шаблон с плейсхолдерами.
     * @param message Сообщение с данными для замены.
     * @return Строка с заменёнными плейсхолдерами.
     */
    std::string replacePlaceholders(const std::string& pattern, const LogMessage& message) const;
};

}  // namespace stc