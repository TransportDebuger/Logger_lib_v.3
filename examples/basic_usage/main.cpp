#include "logger/simple_logger.hpp"
#include "logger/standard_formatter.hpp"
#include "logger/console_output.hpp"
#include "logger/file_output.hpp"
#include "logger/level_filter.hpp"
#include "logger/component_filter.hpp"
#include <iostream>

int main() {
    // Создание логгера с выводом в файл app.log и консоль (консоль добавляется автоматически)
    stc::SimpleLogger logger("app.log");

    // Явно добавляем ещё один канал вывода в консоль
    logger.addOutput(std::make_unique<stc::ConsoleOutput>());

    // Устанавливаем формат сообщений
    auto fmt = std::make_unique<stc::StandardFormatter>("[{level}] {message}");
    logger.setFormatter(std::move(fmt));

    // Установка минимального уровня логирования
    logger.setLevel(stc::LogLevel::Debug);

    // Простейшая запись сообщений разных уровней
    logger.debug("Debug message");
    logger.info("Info message");
    logger.warning("Warning message");
    logger.error("Error message");
    logger.fatal("Fatal message");

    // Демонстрация работы фильтров
    auto lvlFilter = std::make_unique<stc::LevelFilter>(stc::LogLevel::Warning);
    logger.addFilter(std::move(lvlFilter));

    auto compFilter = std::make_unique<stc::ComponentFilter>(stc::ComponentFilter::Mode::Whitelist);
    compFilter->addComponent("Database");
    logger.addFilter(std::move(compFilter));

    // Сообщения после фильтрации
    logger.log({stc::LogLevel::Info, "This info is filtered out", "Database"});
    logger.log({stc::LogLevel::Error, "Database error occurred", "Database"});
    logger.log({stc::LogLevel::Error, "UI error occurred", "UI"}); // отфильтруется

    std::cout << "Demo completed. Check app.log and console output.\n";
    return 0;
}