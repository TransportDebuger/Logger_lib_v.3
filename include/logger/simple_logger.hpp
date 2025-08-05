#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "iformatter.hpp"
#include "ilogger.hpp"
#include "ioutput.hpp"
#include "ifilter.hpp"
#include "log_level.hpp"
#include "log_message.hpp"
#include "standard_formatter.hpp"

namespace stc {

/**
 * @class SimpleLogger
 * @brief Логгер с поддержкой форматирования, нескольких выходов и фильтрации.
 *
 * SimpleLogger позволяет логировать сообщения в различные каналы (консоль, файл и др.)
 * с использованием настраиваемых форматтеров и гибкого набора фильтров.
 *
 * Основные возможности:
 * - Конструктор с параметром имени файла автоматически добавляет FileOutput,
 *   если имя файла задано, иначе добавляется ConsoleOutput.
 * - Методы addOutput(), clearOutputs() для управления каналами вывода.
 * - Форматирование сообщений гибко настраивается через интерфейс IFormatter,
 *   реализация по умолчанию — StandardFormatter.
 * - Цепочка фильтров IFilter (LevelFilter по минимальному уровню установлен по умолчанию).
 * - Методы addFilter(), clearFilters(), getFilterCount() для управления фильтрами.
 * - Методы log(), debug(), info(), warning(), error(), fatal() обеспечивают
 *   удобный и структурированный интерфейс для записи сообщений разных уровней.
 *
 * @note
 * Текущая версия API ориентирована на высокую гибкость и расширяемость за счёт
 * разделения интерфейсов IFormatter и IOutput.
 * 
 * @note Порядок добавления фильтров определяет порядок их применения (Chain of Responsibility).
 *
 * \section deprecated Устаревшие и удалённые методы
 *
 * Для поддержания чистоты кода и однообразия интерфейса был удалён следующий устаревший функционал:
 *
 * - Метод `log(const std::string & message)` отмечен как **deprecated**.
 *   Используйте вместо него `log(LogLevel level, const std::string & message)`.
 *
 * - Метод `logToFile(const std::string & message)` полностью удалён.
 *   Для вывода в файл используйте `addOutput(std::make_unique<FileOutput>(filename))`.
 *
 * - Метод `logToConsole(const std::string & message)` удалён.
 *   Для вывода в консоль добавляйте `ConsoleOutput` через `addOutput`.
 *
 * - Приватные методы `writeToFile` и `writeToConsole` удалены
 *   в пользу единой архитектуры с использованием интерфейса IOutput.
 *
 * \section example Пример использования
 *
 * Пример и подробности использования находятся в файле examples/basic_usage/main.cpp,
 * а также в сопутствующей документации.
 * @example examples/basic_usage/main.cpp
 */
class SimpleLogger : public ILogger {
public:
    /**
     * @brief Конструктор логгера, с выводом сообщений в консоль по умолчанию.
     * @param formatter Форматтер для сообщений (по умолчанию StandardFormatter).
     *
     * Автоматически добавляется вывод в консоль (ConsoleOutput) и
     * базовый фильтр по уровню (LevelFilter с min_level=Debug).
     * 
     * @note В дальнейшем поведение конструктора в части автоматического добавления вывода в консоль может быть изменено.
     */
    explicit SimpleLogger(std::unique_ptr<IFormatter> formatter = std::make_unique<StandardFormatter>());

    /**
     * @brief Конструктор с выводом в файл.
     * @param filename Путь к файлу для логирования.
     * @param formatter Форматтер для сообщений (по умолчанию StandardFormatter).
     * @throws std::runtime_error при невозможности открыть файл.
     * 
     * @note
     * Добавляет консольный вывод вместо файлового в случае передачи пустой строки в аргументе filename. В противном случае консольный вывод не добаляется.
     * Устанавливает базовый LevelFilter(min_level=Debug).
     */
    explicit SimpleLogger(const std::string& filename, std::unique_ptr<IFormatter> formatter = std::make_unique<StandardFormatter>());

    /** @brief Деструктор. Закрывает все выходные потоки. */
    ~SimpleLogger();

    // Реализация интерфейса ILogger

    /**
     * @brief Записать сообщение с указанным уровнем.
     * @param level Уровень логирования.
     * @param message Текст сообщения.
     *
     * Создаёт LogMessage и вызывает log(const LogMessage&).
     */
    void log(LogLevel level, const std::string& message) override;

    /**
     * @brief Установить минимальный уровень логирования.
     * @param min_level Минимальный уровень; все сообщения ниже него игнорируются.
     *
     * Обновляет встроенный LevelFilter.
     */
    void setLevel(LogLevel min_level) override;

    /**
     * @brief Получить текущий минимальный уровень логирования.
     * @return Текущий уровень.
     */
    LogLevel getLevel() const override;

    /**
     * @brief Проверить, разрешено ли логировать сообщение данного уровня.
     * @param level Уровень для проверки.
     * @return true, если сообщение с этим уровнем пройдет через все фильтры.
     *
     * Создаёт тестовое LogMessage с пустым текстом и компонентом.
     */
    bool isEnabled(LogLevel level) const override;

    // Дополнительные методы для работы с LogMessage

    /**
     * @brief Логирование структурированного сообщения.
     * @param log_message Объект с полями level, message, component и пр.
     *
     * Сообщение форматируется и отправляется во все output’ы, если
     * проходит все фильтры.
     */
    void log(const LogMessage& log_message);

    // Методы для работы с выводами

    /**
     * @brief Добавить новый канал вывода сообщений.
     * @param output Уникальный указатель на канал вывода, реализующий интерфейс IOutput.
     */
    void addOutput(std::unique_ptr<IOutput> output);

    /**
     * @brief Очистить все настроенные каналы вывода.
     *
     * После вызова ни один канал не будет получать сообщения до их повторного добавления.
     */
    void clearOutputs();

    // Методы для работы с фильтрами

    /**
     * @brief Добавить фильтр в цепочку фильтрации.
     * @param filter Уникальный указатель на реализацию IFilter.
     *
     * @note
     * Фильтры применяются в порядке добавления; если любой
     * фильтр вернёт false, сообщение не будет обработано.
     */
    void addFilter(std::unique_ptr<IFilter> filter);

    /**
     * @brief Очистить все фильтры и восстановить базовый LevelFilter.
     */
    void clearFilters();

    /**
     * @brief Получить количество активных фильтров.
     * @return Число фильтров в цепочке.
     */
    size_t getFilterCount() const;

    // Управление форматтером

    /**
     * @brief Установить новый форматтер сообщений.
     * @param formatter Новый форматтер сообщений.
     */
    void setFormatter(std::unique_ptr<IFormatter> formatter);

    /**
     * @brief Получить текущий форматтер сообщений.
     * @return Ссылка на текущий форматтер сообщений.
     */
    const IFormatter& getFormatter() const;

private:
    mutable std::mutex mutex_; ///< Мьютекс для потокобезопасности
    std::vector<std::unique_ptr<IOutput>> outputs_; ///< Каналы вывода сообщений
    std::vector<std::unique_ptr<IFilter>> filters_; ///< Фильтры примененные к логгеру
    std::unique_ptr<IFormatter> formatter_; ///< Форматтер для сообщений
    LogLevel min_level_; ///< Минимальный уровень логирования

    /**
     * @brief Проверить сообщение через все фильтры (Chain of Responsibility).
     * @param message Лог-сообщение для проверки.
     * @return true, если ни один фильтр не отклонил сообщение.
     */
    bool shouldProcessMessage(const LogMessage& message) const;
};

}  // namespace stc