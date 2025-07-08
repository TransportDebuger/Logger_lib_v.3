#pragma once

#include "ioutput.hpp"
#include <fstream>
#include <mutex>
#include <string>

namespace stc {

/**
 * @class FileOutput
 * @brief Выводит отформатированное сообщение в файл.
 *
 * Открывает файл в конструкторе и синхронизирует запись через мьютекс.
 */
class FileOutput : public IOutput {
public:
    /**
     * @brief Конструктор.
     * @param filename Путь к файлу для записи.
     * @throws std::runtime_error, если файл не удалось открыть.
     */
    explicit FileOutput(const std::string& filename);

    ~FileOutput();

    /**
     * @brief Вывести сообщение в файл.
     * @param formatted_message Сообщение, уже отформатированное через IFormatter.
     */
    void write(const std::string& formatted_message) override;

private:
    std::ofstream file_stream_;
    std::mutex file_mutex_;
};
} // namespace stc