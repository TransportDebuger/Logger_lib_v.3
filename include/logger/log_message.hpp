/**
 * @file log_message.hpp
 *
 * @author Artem Ulyanov (https://github.com/TransportDebuger)
 * @date 2025-12-14
 * @version 1.1.0
 *
 * @brief Структурированное сообщение для логирования.
 *
 * @details
 * Определяет структуру @ref LogMessage — контейнер для передачи лог-сообщения
 * между компонентами системы (фильтры, форматтеры, выходы).
 *
 * @section log_message_fields Поля структуры
 *
 * - `level`      — уровень логирования (LogLevel)
 * - `thread_id`  — идентификатор потока-источника
 * - `timestamp`  — временная метка создания (std::chrono::system_clock::now())
 * - `text`       — текст сообщения
 * - `component`  — опциональное имя компонента (например, "Database",
 * "Network")
 *
 * @section log_message_design Проектирование
 *
 * - Использована структура с открытыми полями — соответствует идиоме POD.
 * - Поля упорядочены для минимизации padding и улучшения локальности:
 *   ```
 *   LogLevel level (4) + 4 padding → std::thread::id (8) → timestamp (8)
 *   → text (24) → component (24)
 *   ```
 *   Итого: **0 байт потерь на выравнивание**.
 *
 * - Конструкторы:
 *   - `explicit LogMessage(LogLevel, const std::string&, const std::string& =
 * "") noexcept`
 *   - `LogMessage() noexcept` — делегирует основному, уровень по умолчанию:
 * Info
 *
 * - Все конструкторы помечены `noexcept` — безопасны в контейнерах STL.
 * - Конструктор с параметрами — `explicit` — предотвращает неявные
 * преобразования.
 *
 * @warning LogMessage не является потокобезопасным. Однако может безопасно
 * использоваться как значение в потокобезопасных очередях или контейнерах.
 *
 * @section log_message_dependencies Зависимости
 *
 * **Внутренние**:
 * - @ref log_level.hpp — тип LogLevel и вспомогательные функции
 *
 * **Системные**:
 * - `<string>`    — для std::string
 * - `<chrono>`    — для временных меток
 * - `<thread>`    — для std::thread::id
 *
 * @section log_message_related Связанные модули
 *
 * - @ref ILogger      — интерфейс логгера
 * - @ref IFilter      — фильтрация по уровню, компоненту и т.д.
 * - @ref IFormatter   — форматирование сообщений
 * - @ref IOutput      — вывод в консоль, файл, сеть
 * - @ref SimpleLogger — основная реализация
 *
 * @section log_message_changelog История изменений
 *
 * - **v1.1.0 (2025-12-15)**:
 *   - Добавлено описание оптимизации упаковки полей
 *   - Уточнены гарантии `noexcept` и `explicit`
 *   - Исправлена опечатка в названии iformatter.hpp
 *   - Добавлено пояснение про делегирующий конструктор
 *
 * - **v1.0.0 (2025-12-14)**:
 *   - Первоначальная версия
 *
 * @ingroup Core
 * @copyright Copyright (c) 2025 STC Logger Project. All rights reserved.
 */
#pragma once

#include <chrono>
#include <string>
#include <thread>

#include "log_level.hpp"

namespace stc {

/**
 * @struct LogMessage
 * @brief Структурированное сообщение для логгирования.
 *
 * @details
 * `LogMessage` — основная единица данных в системе логирования. Используется
 * для передачи информации между компонентами: фильтры, форматтеры, выходы.
 *
 * Содержит:
 * - Уровень логирования (`level`)
 * - Текст сообщения (`text`)
 * - Временную метку создания (`timestamp`)
 * - Идентификатор потока (`thread_id`)
 * - Имя компонента-источника (`component`, опционально)
 *
 * ### Многопоточность
 *
 * Поддержка многопоточности встроена:
 * - `thread_id` автоматически устанавливается в `std::this_thread::get_id()`
 * - `timestamp` — момент создания сообщения
 * (`std::chrono::system_clock::now()`)
 * - Все поля инициализируются в конструкторе — безопасно для использования в
 * любом потоке
 *
 * ### Конвейер обработки
 *
 * @code
 * logger.info("Database connected", "Database")
 *     ↓
 * Создаётся LogMessage с level=Info, text=..., component="Database"
 *     ↓
 * IFilter::shouldPass() — фильтрация по уровню/компоненту
 *     ↓
 * IFormatter::format() — форматирование с метаданными
 *     ↓
 * IOutput::write() — вывод в консоль, файл и т.д.
 * @endcode
 *
 * ### Гарантии
 * - **Value semantics**: полная копируемость и перемещаемость
 * - **Noexcept**: все конструкторы гарантируют отсутствие исключений
 * - **Thread-safe по значению**: экземпляр безопасно передавать между потоками
 *
 * @note Порядок полей выбран для минимизации padding (0 байт потерь).
 *
 * @see IFilter
 * @see IFormatter
 * @see IOutput
 * @see LogLevel
 *
 * @ingroup Core
 */
struct LogMessage {
  /**
   * @brief Уровень критичности логируемого события.
   *
   * @details
   * Определяет важность события по пятиуровневой шкале:
   *   - Debug   (0) — детальная отладочная информация
   *   - Info    (1) — информационное сообщение о ходе работы
   *   - Warning (2) — потенциально проблемная ситуация
   *   - Error   (3) — ошибка, не приводящая к остановке приложения
   *   - Fatal   (4) — критическая ошибка, возможна аварийная остановка
   *
   * Уровни упорядочены по возрастанию серьёзности. Используется для фильтрации:
   * сообщение логируется, если его уровень не ниже установленного минимального.
   *
   * @see LogLevel для определения перечисления
   * @see shouldLog() для проверки прохождения по уровню
   */
  LogLevel level;

  /**
   * @brief Идентификатор потока, создавшего сообщение.
   *
   * @details
   * Поле автоматически инициализируется в конструкторе с помощью
   * `std::this_thread::get_id()`. Позволяет точно определить, в каком потоке
   * произошло событие — критически важно для отладки и анализа многопоточных
   * приложений.
   *
   * Значение может быть использовано:
   * - В форматировании логов (например, `[Thread: 140235]`)
   * - Для фильтрации сообщений по потокам
   * - При построении трассировки выполнения
   *
   * @note
   * - Устанавливается автоматически, не передаётся в параметрах конструктора.
   * - Гарантированно уникален для каждого потока в пределах процесса.
   *
   * @example
   * ```cpp
   * LogMessage msg(LogLevel::Warning, "Timeout detected");
   * std::cout << "Event from thread: " << msg.thread_id << std::endl;
   * ```
   */
  std::thread::id thread_id;

  /**
   * @brief Время создания сообщения.
   *
   * @details
   * Поле автоматически инициализируется в конструкторе значением
   * `std::chrono::system_clock::now()` — моментом создания объекта
   * `LogMessage`.
   *
   * Используется для:
   * - Хронологического упорядочивания событий в логах
   * - Анализа задержек и производительности
   * - Отладки последовательности операций
   * - Аудита и мониторинга
   * - Корреляции событий между компонентами
   *
   * @note
   * - Устанавливается автоматически, не передаётся в параметрах.
   * - Основано на системных часах, может быть скорректировано при изменении
   * времени ОС.
   * - Для высокоточной внутренней синхронизации рекомендуется `steady_clock`,
   * но для логов предпочтителен `system_clock`.
   *
   * @example
   * ```cpp
   * LogMessage msg(LogLevel::Info, "Operation started");
   * // Расчёт времени, прошедшего с момента логирования
   * auto elapsed = std::chrono::system_clock::now() - msg.timestamp;
   * std::cout << "Прошло: " << elapsed.count() << " наносекунд\n";
   * ```
   */
  std::chrono::system_clock::time_point timestamp;

  /**
   * @brief Текст сообщения.
   *
   * @details
   * Содержит полное описание события:
   * - Простая строка: "Application started"
   * - Информационное: "Connected to database: localhost:5432"
   * - Детальное: "Failed to read config: file not found (errno=2)"
   * - Форматированное: "User authenticated: id=123, name='John'"
   *
   * Может содержать любые символы, но рекомендуется избегать символов
   * новой строки (нарушают форматирование логов).
   *
   * @note
   * - Может быть длинным (нет ограничений)
   * - Используется без изменений при передаче между компонентами
   *
   * @example
   * ```cpp
   * LogMessage msg(LogLevel::Error, "Connection timeout after 30s");
   * ```
   */
  std::string text;

  /**
   * @brief Имя компонента или модуля, создавшего сообщение (опционально).
   *
   * @details
   * Поле предоставляет контекст — указывает, из какой части приложения пришло
   * сообщение. Используется для:
   *   - Фильтрации логов по источникам (например, только "Network")
   *   - Группировки и анализа событий по модулям
   *   - Улучшения читаемости логов (например, "[Database] Connection pool
   * exhausted")
   *   - Диагностики и отладки распределённых компонентов
   *
   * **Рекомендуемые значения**:
   *   - "Database" — модуль работы с базой данных
   *   - "Network"  — сетевые операции, HTTP, сокеты
   *   - "Auth"     — аутентификация и авторизация
   *   - "Config"   — загрузка и обработка конфигурации
   *   - "Main"     — основной поток приложения
   *   - "Worker"   — фоновые задачи или потоки
   *
   * @note
   * - Значение опционально — по умолчанию пустая строка ("").
   * - Не влияет на логику фильтрации по умолчанию, но может использоваться в
   * расширенных фильтрах.
   * - Рекомендуется использовать короткие, понятные имена без пробелов и
   * спецсимволов.
   * - Имя должно быть постоянным в пределах компонента (не генерироваться
   * динамически).
   *
   * @example
   * ```cpp
   * // С указанием компонента
   * LogMessage msg1(LogLevel::Info, "Connected to server", "Network");
   *
   * // Без компонента — значение по умолчанию
   * LogMessage msg2(LogLevel::Warning, "High memory usage");
   *
   * // Использование в условии
   * if (!msg1.component.empty()) {
   *     std::cout << "[" << msg1.component << "] ";
   * }
   * std::cout << msg1.text << std::endl;
   * ```
   */
  std::string component;

  /**
   * @brief Конструктор с параметрами.
   *
   * @param[in] lvl   Уровень логирования (LogLevel::Debug, Info, Warning,
   * Error, Fatal)
   * @param[in] msg   Текст сообщения — копируется в поле `text`
   * @param[in] comp  Имя компонента (опционально, по умолчанию — пустая строка)
   *
   * @details
   * Создаёт объект LogMessage с заданными:
   *   - `level`     — уровнем критичности
   *   - `text`      — текстом сообщения
   *   - `component` — именем источника (если указано)
   *
   * Автоматически инициализируются:
   *   - `timestamp` — текущее время (std::chrono::system_clock::now())
   *   - `thread_id` — идентификатор текущего потока
   * (std::this_thread::get_id())
   *
   * ### Порядок инициализации
   * @code
   * level     = lvl
   * text      = msg
   * component = comp
   * timestamp = std::chrono::system_clock::now()
   * thread_id = std::this_thread::get_id()
   * @endcode
   *
   * ### Производительность
   * - Временная сложность: O(m + c), где `m` — длина `msg`, `c` — длина `comp`
   * - Пространственная: O(m + c) — объём памяти для хранения строк
   * - Вызовы `now()` и `get_id()` — быстрые, но не бесплатные
   *
   * @exception noexcept Гарантирует отсутствие исключений при успешном
   * выделении памяти.
   * @note Конструктор помечен как `explicit` — запрещает неявные
   * преобразования.
   *
   * @example
   * ```cpp
   * // Логирование с указанием компонента
   * LogMessage msg1(LogLevel::Error, "Connection timeout", "Network");
   *
   * // Без компонента — используется значение по умолчанию
   * LogMessage msg2(LogLevel::Info, "Application started");
   *
   * // В многопоточном окружении: thread_id будет корректным
   * std::thread worker([] {
   *     LogMessage msg(LogLevel::Debug, "Worker started", "Worker");
   *     // msg.thread_id == ID нового потока
   *     logger->log(msg);
   * });
   * worker.join();
   * ```
   */
  explicit LogMessage(LogLevel lvl, const std::string& msg,
                      const std::string& comp = "") noexcept
      : level(lvl),
        text(msg),
        component(comp),
        timestamp(std::chrono::system_clock::now()),
        thread_id(std::this_thread::get_id()) {}

  /**
   * @brief Конструктор по умолчанию.
   *
   * @details
   * Создаёт объект LogMessage, делегируя инициализацию параметризованному
   * конструктору:
   * @code
   * LogMessage() : LogMessage(LogLevel::Info, "", "") {}
   * @endcode
   *
   * ### Значения полей
   *   - `level`     = LogLevel::Info
   *   - `text`      = "" (пустая строка)
   *   - `component` = "" (пустая строка)
   *   - `timestamp` = std::chrono::system_clock::now() — момент создания
   * объекта
   *   - `thread_id` = std::this_thread::get_id() — ID потока, в котором создан
   * объект
   *
   * ### Типичные случаи использования
   *   - Временные или заполнительные сообщения при инициализации
   *   - Создание контейнеров фиксированного размера:
   *     ```cpp
   *     std::vector<LogMessage> buffer(100); // 100 сообщений по умолчанию
   *     ```
   *   - Переменные, которые будут перезаписаны позже:
   *     ```cpp
   *     LogMessage msg;
   *     if (condition) {
   *         msg = LogMessage(LogLevel::Warning, "Low battery", "Power");
   *     }
   *     ```
   *
   * @note
   * - Помечен как `noexcept` — безопасен в контейнерах STL.
   * - `thread_id` и `timestamp` соответствуют потоку и времени **создания
   * объекта**, а не присваивания.
   * - Рекомендуется явно инициализировать сообщение при логировании —
   * конструктор по умолчанию не предназначен для прямого использования в
   * `logger.log(...)`.
   *
   * @example
   * ```cpp
   * // Проверка значений по умолчанию
   * LogMessage msg;
   * assert(msg.level == LogLevel::Info);
   * assert(msg.text.empty());
   * assert(msg.component.empty());
   *
   * // Использование в очереди
   * std::queue<LogMessage> message_queue;
   * message_queue.push(LogMessage{}); // Сообщение с level=Info
   * ```
   */
  LogMessage() noexcept : LogMessage(LogLevel::Info, "", "") {}
};

}  // namespace stc