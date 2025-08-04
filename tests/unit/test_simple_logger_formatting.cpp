#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <regex>
#include "logger/simple_logger.hpp"
#include "logger/standard_formatter.hpp"
#include "logger/console_output.hpp"
#include "logger/file_output.hpp"

using namespace stc;

// Класс для перехвата cout
class CoutRedirect {
public:
    CoutRedirect(std::streambuf* new_buffer) : old(std::cout.rdbuf(new_buffer)) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
private:
    std::streambuf* old;
};

TEST(SimpleLoggerFormattingTest, ConsoleOutputUsesFormatter) {
    // Форматтер с простым шаблоном
    auto fmt = std::make_unique<StandardFormatter>("{level}:{message}");
    SimpleLogger logger(std::move(fmt));
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.log(LogLevel::Warning, "Warn");
    
    std::string output = ss.str();
    EXPECT_NE(output.find("WARNING:Warn"), std::string::npos);
}

TEST(SimpleLoggerFormattingTest, FileOutputUsesFormatter) {
    const std::string filename = "test_fmt.log";
    auto fmt = std::make_unique<StandardFormatter>("{message}");
    SimpleLogger logger(filename, std::move(fmt));
    
    logger.log(LogLevel::Info, "InfoMsg");
    
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "InfoMsg");
    file.close();
    
    std::remove(filename.c_str());
}

TEST(SimpleLoggerFormattingTest, LevelFilteringWorks) {
    auto fmt = std::make_unique<StandardFormatter>("{level}");
    SimpleLogger logger(std::move(fmt));
    logger.setLevel(LogLevel::Error);
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    // Info не должно проходить
    logger.info("Ignored");
    EXPECT_TRUE(ss.str().empty());
    
    // Error должно проходить
    logger.error("Shown");
    EXPECT_NE(ss.str().find("ERROR"), std::string::npos);
}

TEST(SimpleLoggerFormattingTest, MultipleOutputsWork) {
    const std::string filename = "test_multiple.log";
    SimpleLogger logger;
    logger.clearOutputs();
    
    // Добавляем и консольный, и файловый вывод
    logger.addOutput(std::make_unique<ConsoleOutput>());
    logger.addOutput(std::make_unique<FileOutput>(filename));
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.log(LogLevel::Info, "Multiple outputs test");
    
    // Проверяем консольный вывод
    std::string console_output = ss.str();
    EXPECT_NE(console_output.find("Multiple outputs test"), std::string::npos);
    
    // Проверяем файловый вывод
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string file_line;
    std::getline(file, file_line);
    EXPECT_NE(file_line.find("Multiple outputs test"), std::string::npos);
    file.close();
    
    std::remove(filename.c_str());
}