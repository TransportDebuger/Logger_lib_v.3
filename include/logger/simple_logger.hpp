#pragma once

#include <string>
#include <mutex>
#include <fstream>

namespace stc {

/**
 * @brief Простейший синхронный логгер.
 *
 * Позволяет выводить сообщения в консоль и (опционально) в файл.
 */
class SimpleLogger {
public:
    /**
     * @brief Конструктор.
     * @param filename Путь к файлу для логирования. Пустая строка — только консоль.
     * @throws std::runtime_error при невозможности открыть файл.
     */
    explicit SimpleLogger(const std::string& filename = "");

    /** @brief Деструктор закрывает файл (если открыт). */
    ~SimpleLogger();

    /**
     * @brief Записать сообщение в консоль.
     * @param message Текст сообщения.
     */
    void log(const std::string& message);

    /**
     * @brief Записать сообщение в файл.
     * @param message Текст сообщения.
     * @throws std::runtime_error, если файл не открыт.
     */
    void logToFile(const std::string& message);

private:
    std::mutex mutex_;
    std::ofstream file_stream_;
};

} // namespace advanced_logger