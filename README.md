# Logger_lib v3

Хобби-проект библиотеки для логирования на C++ с поддержкой Python, разработываемый с упором на современный, гибкий и высокопроизводительный подход.

---

## Планируемые возможности

- Реализация на C++20 с использованием новейших возможностей языка.
- Высокопроизводительное асинхронное и синхронное логирование.
- Множество стратегий вывода: консоль, файл, сеть и расширяемые каналы.
- Гибкая фильтрация по уровням и категориям.
- Поддержка настраиваемых форматов сообщений через интерфейс `IFormatter`.
- Потокобезопасная реализация с помощью мьютексов.
- Интеграция с Python через актуальные биндинги.
- Мощная и удобная система конфигурации (JSON, YAML).
- Модульная архитектура, отвечающая принципам SOLID.

---

## Статус и этапы разработки

Проект постепенно развивается. Планируемая этапность проведения работ:

- **Этап 1**: Основные компоненты и архитектура — ядро, уровни логов, форматтеры, базовые выводы.
- **Этап 2**: Расширенные возможности — добавление новых стратегий вывода, динамическое подключение выводов, интеграция с Python.
- **Этап 3**: Конфигурирование и усовершенствование производительности, готовность к использованию в реальных продуктах.

В настоящее время реализуются компненты предусмотренные первым этапом проекта.

---

## Пример использования

```cpp
#include "logger.hpp"
#include "standard_formatter.hpp"
#include "console_output.hpp"
#include "file_output.hpp"
#include <iostream>

int main() {
stc::SimpleLogger logger("app.log");
logger.setLevel(stc::LogLevel::Debug);
// Простейшая запись сообщений различных уровней
logger.debug("Debug message");
logger.info("Info message");
logger.warning("Warning message");
logger.error("Error message");
logger.fatal("Fatal message");

// Создание кастомного логгера с пользовательским форматом и несколькими выводами
auto formatter = std::make_unique<stc::StandardFormatter>(
    "{level}: {message} (thread: {thread_id})"
);

stc::SimpleLogger customLogger;
customLogger.setFormatter(std::move(formatter));
customLogger.addOutput(std::make_unique<stc::ConsoleOutput>());
customLogger.addOutput(std::make_unique<stc::FileOutput>("custom.log"));

customLogger.info("Custom formatted message");

// Запись структурированного сообщения
stc::LogMessage structuredMsg(stc::LogLevel::Warning, "Structured message", "MyComponent");
logger.log(structuredMsg);

std::cout << "Logging completed. Check output files." << std::endl;
return 0;
}

```

---

## Сборка проекта

mkdir build
cd build
cmake .. -DBUILD_EXAMPLES=ON -DBUILD_TESTS=ON
make -j$(nproc)


Запуск тестов:

ctest --output-on-failure

Запуск примеров:

./bin/examples/basic_usage

---

## Документация

Полный API и описания доступны в сгенерированной Doxygen документации в папке `docs/html`.

---

## Контакты и поддержка

Для вопросов и предложений используйте раздел Issues на GitHub.