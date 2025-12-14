/**
 * @file ilogger.hpp
 *
 * @author Artem Ulyanov (https://github.com/TransportDebuger)
 * @date 2025-12-15
 * @version 2.1
 *
 * @brief Интерфейс логгера — абстрактный контракт для всех реализаций.
 *
 * Определяет минимальный набор операций, обязательных для всех логгеров
 * в системе Logger_lib. Интерфейс соответствует принципам SOLID,
 * обеспечивает тестируемость и расширяемость.
 *
 * @section ilogger_interface Методы интерфейса
 *
 * - @ref log() — запись сообщения с проверкой уровня
 * - @ref setLevel() — установка минимального уровня логирования
 * - @ref getLevel() — получение текущего уровня
 * - @ref isEnabled() — проверка, будет ли сообщение залогировано
 *
 * @section ilogger_design Архитектура
 *
 * Интерфейс содержит только чистые виртуальные методы:
 * - Нет реализации поведения — только контракт
 * - Обеспечивает разделение ответственностей
 * - Поддерживает ISP и LSP (SOLID)
 * - Упрощает создание mock-объектов для тестов
 *
 * @section ilogger_dependencies Зависимости
 *
 * - `<string>` — для `std::string`
 * - `"log_level.hpp"` — для `LogLevel` и вспомогательных функций
 *
 * @section ilogger_history История
 *
 * - **v2.1** (2025-12-15): Улучшена документация `isEnabled()`, устранено дублирование `@ingroup`
 * - **v2.0** (2025-12-14): Полная рефакторизация интерфейса, удалены реализации, добавлены `[[nodiscard]]` и `noexcept`
 * - **v1.0** (2025-12-13): Первоначальная версия
 *
 * @section ilogger_related Связанные компоненты
 *
 * - @ref Core — основные компоненты системы
 * - @ref SimpleLogger — основная реализация интерфейса
 * - @ref LogLevel — уровни логирования
 * - @ref shouldLog() — глобальная функция фильтрации
 *
 * @ingroup Core
 * @copyright Copyright (c) 2025 STC Logger Project. All rights reserved.
 */

#pragma once

#include <string>

#include "log_level.hpp"

namespace stc {

/**
 * @class ILogger
 * @brief Абстрактный интерфейс логгера.
 *
 * Определяет базовые операции для систем логирования.
 * Все реализации должны наследовать этот интерфейс.
 *
 * @note Интерфейс содержит только чистые виртуальные методы.
 * @see SimpleLogger — пример реализации
 * @ingroup Core
 */
class ILogger {
 public:
  /// Виртуальный деструктор с гарантией отсутствия исключений.
  virtual ~ILogger() noexcept = default;

  /**
     * @brief Записывает сообщение, если оно проходит фильтр уровня.
     * @param level Уровень сообщения.
     * @param msg Текст сообщения.
     *
     * Реализация должна:
     * - Проверить, разрешён ли уровень через isEnabled()
     * - Отформатировать и вывести сообщение, если разрешено
     *
     * @see isEnabled() — для проверки без логирования
     * @see shouldLog() — глобальная функция фильтрации
     */
  virtual void log(LogLevel level, const std::string& msg) = 0;

  /**
     * @brief Устанавливает минимальный уровень логирования.
     * @param min_level Сообщения ниже этого уровня будут отклонены.
     *
     * @see getLevel() — для получения текущего уровня
     * @see isEnabled() — для проверки конкретного уровня
     */
  virtual void setLevel(LogLevel min_level) = 0;

  /**
     * @brief Возвращает текущий минимальный уровень логирования.
     * @return Уровень, установленный через setLevel().
     * @note Результат помечен [[nodiscard]] — игнорирование вызовет предупреждение компилятора.
     */
  [[nodiscard]] virtual LogLevel getLevel() const = 0;

  /**
     * @brief Проверяет, будет ли сообщение с указанным уровнем залогировано.
     * @param level Уровень сообщения.
     * @return true, если `level >= текущего минимального уровня`.
     *
     * Используется для оптимизации: позволяет избежать дорогостоящего
     * форматирования при отключённом уровне.
     *
     * Пример:
     * @code
     * if (logger.isEnabled(LogLevel::Debug)) {
     *     logger.log(LogLevel::Debug, expensiveFormat(data));
     * }
     * @endcode
     *
     * @see shouldLog() — независимая функция с той же логикой
     */
  [[nodiscard]] virtual bool isEnabled(LogLevel level) const = 0;
};

}  // namespace stc