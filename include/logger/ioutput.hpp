#pragma once

#include <string>

namespace stc {

/**
 * @class IOutput
 * @brief Абстрактный интерфейс вывода отформатированных сообщений.
 *
 * Представляет собой стратегию, определяющую, куда и как выводить
 * уже отформатированную строку лога.
 */
class IOutput {
public:
    virtual ~IOutput() = default;

    /**
     * @brief Вывести отформатированное сообщение.
     * @param formatted_message Строка, полученная от IFormatter::format().
     */
    virtual void write(const std::string& formatted_message) = 0;
};

} // namespace stc