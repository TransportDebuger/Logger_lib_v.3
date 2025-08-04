\page simple_logger_example Пример использования SimpleLogger

Ниже приведён минимальный пример использования `stc_logger::SimpleLogger`:

```cpp
#include "logger/simple_logger.hpp"
#include "logger/standard_formatter.hpp"
#include "logger/console_output.hpp"
#include "logger/file_output.hpp"
#include <iostream>

int main() {
    // Создание логгера с выводом в файл app.log и консоль (консоль добавляется автоматически)
    stc::SimpleLogger logger("app.log");
    // Установка минимального уровня логирования
    logger.setLevel(stc::LogLevel::Debug);

    // Логирование сообщений разных уровней
    logger.debug("Debug message");
    logger.info("Info message");
    logger.warning("Warning message");
    logger.error("Error message");
    logger.fatal("Fatal message");

    // Создание логгера с кастомным форматтером и добавлением выходов вручную
    auto custom_formatter = std::make_unique<stc::StandardFormatter>(
        "{level}: {message} (thread: {thread_id})"
    );
    stc::SimpleLogger custom_logger;
    custom_logger.setFormatter(std::move(custom_formatter));
    custom_logger.addOutput(std::make_unique<stc::ConsoleOutput>());
    custom_logger.addOutput(std::make_unique<stc::FileOutput>("custom.log"));

    custom_logger.info("Custom formatted message");

    // Логирование структурированного сообщения
    stc::LogMessage structured_msg(stc::LogLevel::Warning, "Structured message", "MyComponent");
    logger.log(structured_msg);

    std::cout << "Logger demo completed. Check app.log and custom.log files.\n";
    return 0;
}
```

Описание работы:
- Конструктор с именем файла автоматически добавляет вывод в файл.
- В случае использования конструктора по умолчанию, или если в констурктор передано пустое имя файла автоматически добавляется вывод в консоль. В дальнейшем данное поведение может быть изменено и потребовать явного создания канала вывода с использованием `addOutput()`.
- Для вывода на несколько каналов можно использовать `addOutput()` и добавить кастомные выходы.
- Форматирование сообщений настраивается через интерфейс `IFormatter` и реализуется в `StandardFormatter`.


Содержимое примера (`examples/basic_usage/main.cpp`) можно посмотреть [здесь](basic_usage/main.cpp).

Скомпилировать и запустить:<br>
    mkdir build && cd build<br>
    cmake .. -DBUILD_EXAMPLES=ON<br>
    make basic_usage<br>
    ./bin/basic_usage/basic_usage