#pragma once

#include "ioutput.hpp"
#include <mutex>
#include <iostream>

namespace stc {

/**
 * @class ConsoleOutput
 * @brief Выводит отформатированное сообщение в консоль (std::cout).
 *
 * Потокобезопасен за счёт внутреннего мьютекса.
 */
class ConsoleOutput : public IOutput {
public:
    /**
     * @brief Вывести сообщение в консоль.
     * @param formatted_message Сообщение, уже отформатированное через IFormatter.
     */
    void write(const std::string& formatted_message) override;

private:
    /// Мьютекс для синхронизации доступа к std::cout
    static std::mutex cout_mutex_;
};

} // namespace stc