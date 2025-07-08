#pragma once

#include <string>

#include "log_message.hpp"

namespace stc {

/**
 * @class IFormatter
 * @brief Абстрактный интерфейс для форматирования сообщений логгирования.
 *
 * Определяет контракт для преобразования структурированного сообщения
 * LogMessage в строку для вывода или сохранения.
 */
class IFormatter {
public:
    /**
     * @brief Виртуальный деструктор для корректного удаления наследников.
     */
    virtual ~IFormatter() = default;

    /**
     * @brief Форматирует сообщение логгирования в строку.
     * @param message Структурированное сообщение для форматирования.
     * @return Отформатированная строка, готовая для вывода.
     *
     * Этот метод должен быть реализован в каждом конкретном форматтере
     * для определения собственного стиля форматирования.
     */
    virtual std::string format(const LogMessage& message) = 0;
};

}  // namespace stc