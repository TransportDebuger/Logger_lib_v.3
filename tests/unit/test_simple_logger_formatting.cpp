#include <gtest/gtest.h>

#include <fstream>
#include <regex>
#include <sstream>

#include "logger/log_message.hpp"
#include "logger/simple_logger.hpp"

using namespace stc;

// Перехватываем вывод в консоль
class CoutRedirect {
public:
    CoutRedirect(std::streambuf* new_buffer) : old(std::cout.rdbuf(new_buffer)) {}
    ~CoutRedirect() {
        std::cout.rdbuf(old);
    }

private:
    std::streambuf* old;
};

TEST(SimpleLoggerFormattingTest, ConsoleOutputUsesFormatter) {
    // Подготовка: форматтер с явно понятным шаблоном
    auto fmt = std::make_unique<StandardFormatter>("{level}:{message}");
    SimpleLogger logger(std::move(fmt));

    // Перенаправление cout
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());

    logger.log(LogLevel::Warning, "Warn");
    std::string out = ss.str();
    EXPECT_NE(out.find("WARNING:Warn"), std::string::npos);
}

TEST(SimpleLoggerFormattingTest, FileOutputUsesFormatter) {
    const std::string filename = "test_fmt.log";
    // Используем шаблон без компонентов для простоты
    auto fmt = std::make_unique<StandardFormatter>("{message}");
    SimpleLogger logger(filename, std::move(fmt));

    // Запись и проверка содержимого файла
    logger.log(LogLevel::Info, "InfoMsg");
    std::ifstream f(filename);
    ASSERT_TRUE(f.is_open());
    std::string line;
    std::getline(f, line);
    EXPECT_EQ(line, "InfoMsg");
    f.close();
    std::remove(filename.c_str());
}

TEST(SimpleLoggerFormattingTest, LevelFilteringWorks) {
    auto fmt = std::make_unique<StandardFormatter>("{level}");
    SimpleLogger logger(std::move(fmt));
    logger.setLevel(LogLevel::Error);

    // Перехватим cout
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());

    logger.info("Ignored");
    EXPECT_TRUE(ss.str().empty());

    logger.error("Shown");
    EXPECT_NE(ss.str().find("ERROR"), std::string::npos);
}