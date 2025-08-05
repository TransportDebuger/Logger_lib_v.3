#pragma once

#include "log_message.hpp"

namespace stc {

/**
 * @interface IFilter
 * @brief Интерфейс для фильтрации логов (Strategy Pattern).
 *
 * Паттерн Strategy позволяет динамически выбирать алгоритм фильтрации
 * сообщений. Каждый класс, реализующий IFilter, предоставляет свою
 * стратегию (критерий) для решения, следует ли обрабатывать сообщение дальше.
 *
 * Использование:
 * - Добавьте фильтр в SimpleLogger через addFilter().
 * - Сообщение будет проходить через все фильтры в порядке добавления.
 *
 * @example
 * // Пример добавления фильтра в SimpleLogger
 * auto levelFilter = std::make_unique<LevelFilter>(LogLevel::Warning);
 * auto componentFilter = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Whitelist);
 * componentFilter->addComponent("Database");
 *
 * SimpleLogger logger;
 * logger.addFilter(std::move(levelFilter));
 * logger.addFilter(std::move(componentFilter));
 *
 * logger.info("Info message");      // Может быть отфильтровано, если не соответствует критериям
 * logger.error("Error message");    // Если проходит все фильтры — будет выведено
 */
class IFilter {
public:
    virtual ~IFilter() = default;

    /**
     * @brief Проверить, разрешено ли прохождение сообщения.
     * @param message Ссылка на структурированное сообщение LogMessage.
     * @return true, если сообщение должно быть передано дальше по цепочке,
     *         false — если сообщение нужно отбросить.
     */
    virtual bool shouldPass(const LogMessage& message) const = 0;
};

} // namespace stc