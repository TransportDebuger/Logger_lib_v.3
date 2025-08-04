#include "logger/simple_logger.hpp"
#include "logger/standard_formatter.hpp"
#include "logger/file_output.hpp"
#include <iostream>

int main() {
    // Создание логгера. ConsoleOutput добавляется автоматически
    stc::SimpleLogger logger("app.log");

    // Установка минимального уровня логирования
    logger.setLevel(stc::LogLevel::Debug);

    // Использование удобных методов логирования
    logger.debug("Debug message");
    logger.info("Info message");
    logger.warning("Warning message");
    logger.error("Error message");
    logger.fatal("Fatal message");

    // Создание логгера с кастомным форматтером
    auto custom_formatter = std::make_unique<stc::StandardFormatter>(
        "{level}: {message} (thread: {thread_id})"
    );
    stc::SimpleLogger custom_logger;
    custom_logger.setFormatter(std::move(custom_formatter));
    // FileOutput можно добавить дополнительно по желанию
    custom_logger.addOutput(std::make_unique<stc::FileOutput>("custom.log"));

    custom_logger.info("Custom formatted message");

    // Работа со структурированным сообщением
    stc::LogMessage structured_msg(stc::LogLevel::Warning, "Structured message", "MyComponent");
    logger.log(structured_msg);

    std::cout << "Logger demo completed. Check app.log and custom.log files.\n";
    return 0;
}