#pragma once

#include "ifilter.hpp"
#include <string>
#include <unordered_set>

namespace stc {

/**
 * @class ComponentFilter
 * @brief Фильтр по компоненту/модулю (Strategy Pattern).
 *
 * Данный фильтр позволяет пропускать или блокировать лог-сообщения
 * на основе их поля component. Имеет два режима работы:
 * - Whitelist: пропускает только указанные компоненты (или все при пустом списке).
 * - Blacklist: блокирует только указанные компоненты (пропускает все остальные).
 *
 * Использование:
 * @code
 * auto filter = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Whitelist);
 * filter->addComponent("Database");
 * filter->addComponent("Network");
 *
 * SimpleLogger logger;
 * logger.addFilter(std::move(filter));
 *
 * logger.log(LogLevel::Info, "Message", "Database"); // Пропуск
 * logger.log(LogLevel::Info, "Message", "UI");       // Блокировка
 * @endcode
 *
 * @see IFilter
 */
class ComponentFilter : public IFilter {
public:
    /**
     * @enum Mode
     * @brief Режим работы фильтра.
     */
    enum class Mode {
        Whitelist, ///< Пропускать только указанные компоненты
        Blacklist  ///< Блокировать только указанные компоненты
    };

    /**
     * @brief Конструктор.
     * @param mode Режим фильтрации (Whitelist или Blacklist).
     */
    explicit ComponentFilter(Mode mode = Mode::Whitelist);

    /**
     * @brief Проверить, должно ли сообщение пройти фильтр.
     * @param message Объект LogMessage с данными о сообщении.
     * @return true, если сообщение должно быть обработано дальше.
     */
    bool shouldPass(const LogMessage& message) const override;

    /**
     * @brief Добавить компонент в список фильтрации.
     * @param component Имя компонента.
     */
    void addComponent(const std::string& component);

    /**
     * @brief Удалить компонент из списка.
     * @param component Название компонента.
     */
    void removeComponent(const std::string& component);

    /**
     * @brief Очистить список компонентов.
     */
    void clearComponents();

    /**
     * @brief Получить текущий режим работы фильтра.
     * @return Значение Mode (Whitelist или Blacklist).
     */
    Mode getMode() const;

private:
    Mode mode_; ///< Режим работы фильтра
    std::unordered_set<std::string> components_; ///< Набор компонентов для фильтрации
};

} // namespace stc