#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>

#include "iformatter.hpp"
#include "ilogger.hpp"
#include "log_level.hpp"
#include "log_message.hpp"
#include "standard_formatter.hpp"

namespace stc {

/**
 * @class SimpleLogger
 * @brief Простейший синхронный логгер с поддержкой форматирования.
 *
 * Позволяет выводить сообщения в консоль и файл с использованием
 * настраиваемых форматтеров.
 *
 * @example basic_usage/main.cpp
 * Пример использования SimpleLogger с форматированием.
 *
 * Демонстрирует работу с разными уровнями логирования,
 * настраиваемыми форматтерами и структурированными сообщениями.
 *
 * Скомпилировать и запустить:
 *          mkdir build && cd build
 *          cmake .. -DBUILD_EXAMPLES=ON
 *          make basic_usage
 *          ./bin/basic_usage/basic_usage
 *
 */
class SimpleLogger : public ILogger {
public:
    /**
     * @brief Конструктор только для консольного вывода.
     * @param formatter Форматтер для сообщений (по умолчанию StandardFormatter).
     */
    explicit SimpleLogger(
        std::unique_ptr<IFormatter> formatter = std::make_unique<StandardFormatter>());

    /**
     * @brief Конструктор с выводом в файл.
     * @param filename Путь к файлу для логирования.
     * @param formatter Форматтер для сообщений (по умолчанию StandardFormatter).
     * @throws std::runtime_error при невозможности открыть файл.
     */
    explicit SimpleLogger(const std::string& filename, std::unique_ptr<IFormatter> formatter =
                                                           std::make_unique<StandardFormatter>());

    /** @brief Деструктор закрывает файл (если открыт). */
    ~SimpleLogger();

    // Реализация интерфейса ILogger

    /**
     * @brief Записать сообщение с указанным уровнем.
     * @param level Уровень логирования.
     * @param message Текст сообщения.
     */
    void log(LogLevel level, const std::string& message) override;

    /**
     * @brief Установить минимальный уровень логирования.
     * @param min_level Новый минимальный уровень.
     */
    void setLevel(LogLevel min_level) override;

    /**
     * @brief Получить текущий минимальный уровень логирования.
     * @return Текущий уровень.
     */
    LogLevel getLevel() const override;

    /**
     * @brief Проверить, разрешено ли логировать сообщение с данным уровнем.
     * @param level Проверяемый уровень.
     * @return true, если разрешено.
     */
    bool isEnabled(LogLevel level) const override;

    // Дополнительные методы для работы с LogMessage

    /**
     * @brief Записать структурированное сообщение.
     * @param log_message Структурированное сообщение.
     */
    void log(const LogMessage& log_message);

    /**
     * @brief Записать сообщение в файл.
     * @param log_message Структурированное сообщение.
     * @throws std::runtime_error, если файл не открыт.
     */
    void logToFile(const LogMessage& log_message);

    // Методы для обратной совместимости

    /**
     * @brief Записать сообщение в консоль (устаревший метод).
     * @param message Текст сообщения.
     * @deprecated Используйте log(LogLevel, const std::string&) вместо этого.
     */
    void log(const std::string& message);

    /**
     * @brief Записать сообщение в файл (устаревший метод).
     * @param message Текст сообщения.
     * @deprecated Используйте logToFile(const LogMessage&) вместо этого.
     */
    void logToFile(const std::string& message);

    // Управление форматтером

    /**
     * @brief Установить новый форматтер.
     * @param formatter Новый форматтер.
     */
    void setFormatter(std::unique_ptr<IFormatter> formatter);

    /**
     * @brief Получить текущий форматтер.
     * @return Ссылка на текущий форматтер.
     */
    const IFormatter& getFormatter() const;

private:
    mutable std::mutex mutex_;
    std::ofstream file_stream_;
    std::unique_ptr<IFormatter> formatter_;
    LogLevel min_level_;

    /**
     * @brief Внутренний метод для записи отформатированного сообщения.
     * @param formatted_message Отформатированная строка.
     */
    void writeToConsole(const std::string& formatted_message);

    /**
     * @brief Внутренний метод для записи отформатированного сообщения в файл.
     * @param formatted_message Отформатированная строка.
     */
    void writeToFile(const std::string& formatted_message);
};

}  // namespace stc