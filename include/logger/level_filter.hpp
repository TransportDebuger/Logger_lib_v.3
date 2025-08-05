#pragma once

#include "ifilter.hpp"
#include "log_level.hpp"

namespace stc {

/**
 * @class LevelFilter
 * @brief Стратегия фильтрации по уровню логирования.
 *
 * Реализует паттерн Strategy. Пропускает только те сообщения,
 * уровень которых не ниже заданного минимального уровня.
 *
 * Использование:
 * @code
 * // Фильтр, пропускающий сообщения уровня Warning и выше
 * auto filter = std::make_unique<LevelFilter>(LogLevel::Warning);
 * SimpleLogger logger;
 * logger.addFilter(std::move(filter));
 * @endcode
 *
 * @see IFilter
 */
class LevelFilter : public IFilter {
public:
    /**
     * @brief Конструктор.
     * @param min_level Минимальный уровень, при котором сообщение будет пропущено.
     */
    explicit LevelFilter(LogLevel min_level);

    /**
     * @brief Проверить, проходит ли сообщение по уровню.
     * @param message Лог-сообщение для проверки.
     * @return true, если message.level >= min_level_, иначе false.
     */
    bool shouldPass(const LogMessage& message) const override;

    /**
     * @brief Получить текущий минимальный уровень фильтрации.
     * @return Значение уровня min_level_.
     */
    LogLevel getMinLevel() const;

    /**
     * @brief Установить новый минимальный уровень фильтрации.
     * @param min_level Новый минимальный уровень для пропуска сообщений.
     */
    void setMinLevel(LogLevel min_level);

private:
    LogLevel min_level_; ///< Минимальный уровень для пропуска сообщений
};

} // namespace stc