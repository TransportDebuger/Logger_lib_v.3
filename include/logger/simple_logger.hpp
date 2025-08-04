#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "iformatter.hpp"
#include "ilogger.hpp"
#include "ioutput.hpp"
#include "log_level.hpp"
#include "log_message.hpp"
#include "standard_formatter.hpp"

namespace stc {

/**
 * @class SimpleLogger
 * @brief Простейший синхронный логгер с поддержкой форматирования и множественных выходов.
 *
 * SimpleLogger позволяет логировать сообщения в различные каналы
 * (консоль, файлы и др.) с использованием настраиваемых форматтеров.
 *
 * Основные возможности:
 * - Конструктор с параметром имени файла автоматически добавляет FileOutput,
 *   если имя файла задано, иначе добавляется ConsoleOutput.
 * - Метод addOutput() позволяет добавить дополнительные каналы вывода.
 * - Методы log(), debug(), info(), warning(), error(), fatal() обеспечивают
 *   удобный и структурированный интерфейс для записи сообщений разных уровней.
 * - Форматирование сообщений гибко настраивается через интерфейс IFormatter,
 *   реализация по умолчанию — StandardFormatter.
 *
 * @note
 * Текущая версия API ориентирована на высокую гибкость и расширяемость за счёт
 * разделения интерфейсов IFormatter и IOutput.
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
 */
class SimpleLogger : public ILogger {
public:
    /**
     * @brief Конструктор логгера, с выводом сообщений в консоль по умолчанию.
     * @param formatter Форматтер для сообщений (по умолчанию StandardFormatter).
     *
     * Автоматически добавляется вывод в консоль (ConsoleOutput).
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
     */
    explicit SimpleLogger(const std::string& filename, std::unique_ptr<IFormatter> formatter = std::make_unique<StandardFormatter>());

    /** @brief Деструктор закрывает файл (если открыт). */
    ~SimpleLogger();

    // Реализация интерфейса ILogger

    /**
     * @brief Записать сообщение с указанным уровнем.
     * @param level Уровень логирования.
     * @param message Текст сообщения.
     */
    void log(LogLevel level, const std::string& message) override;

    /**
     * @brief Установить минимальный уровень логирования.
     * @param min_level Новый минимальный уровень.
     */
    void setLevel(LogLevel min_level) override;

    /**
     * @brief Получить текущий минимальный уровень логирования.
     * @return Текущий уровень.
     */
    LogLevel getLevel() const override;

    /**
     * @brief Проверить, разрешено ли логировать сообщение с данным уровнем.
     * @param level Проверяемый уровень.
     * @return true, если разрешено.
     */
    bool isEnabled(LogLevel level) const override;

    // Дополнительные методы для работы с LogMessage

    /**
     * @brief Записать структурированное сообщение.
     * @param log_message Структурированное сообщение.
     * 
     * @deprecated Рекомендуется использовать log(LogLevel, const std::string&).
     * 
     * @note 
     * Простой API для базового использования.
     * Оставлен для обратной совместимости. 
     */
    void log(const LogMessage& log_message);

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
    std::unique_ptr<IFormatter> formatter_; ///< Форматтер для сообщений
    LogLevel min_level_; ///< Минимальный уровень логирования
};

}  // namespace stc