#include "logger/simple_logger.hpp"
#include "logger/standard_formatter.hpp"
#include <iostream>

int main() {
    // Создание логгера с стандартным форматтером
    stc::SimpleLogger logger("app.log");
    
    // Установка минимального уровня логирования
    logger.setLevel(stc::LogLevel::Debug);
    
    // Использование удобных методов
    logger.debug("Debug message");
    logger.info("Info message");
    logger.warning("Warning message");
    logger.error("Error message");
    logger.fatal("Fatal message");
    
    // Создание логгера с настраиваемым форматтером
    auto custom_formatter = std::make_unique<stc::StandardFormatter>(
        "{level}: {message} (thread: {thread_id})"
    );
    
    stc::SimpleLogger custom_logger("custom.log", std::move(custom_formatter));
    custom_logger.info("Custom formatted message");
    
    // Работа со структурированными сообщениями
    stc::LogMessage structured_msg(stc::LogLevel::Warning, "Structured message", "MyComponent");
    logger.log(structured_msg);
    
    std::cout << "Logger demo completed. Check app.log and custom.log files.\n";
    return 0;
}