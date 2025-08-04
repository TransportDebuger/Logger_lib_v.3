#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "logger/simple_logger.hpp"
#include "logger/console_output.hpp"
#include "logger/file_output.hpp"

using namespace stc;

class CoutRedirect {
public:
    CoutRedirect(std::streambuf* new_buffer) : old(std::cout.rdbuf(new_buffer)) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
private:
    std::streambuf* old;
};

TEST(SimpleLoggerTest, DefaultConstructorAddsConsoleOutput) {
    SimpleLogger logger;
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.log(LogLevel::Info, "Test message");
    
    std::string output = ss.str();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("Test message"), std::string::npos);
}

TEST(SimpleLoggerTest, FileConstructorAddsFileOutput) {
    const std::string filename = "test_file_only.log";
    SimpleLogger logger(filename);
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.log(LogLevel::Info, "File message");
    
    EXPECT_TRUE(ss.str().empty());
    
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::getline(file, line);
    EXPECT_NE(line.find("File message"), std::string::npos);
    file.close();

    std::remove(filename.c_str());
}

TEST(SimpleLoggerTest, EmptyFilenameConstructorAddsConsoleOutput) {
    SimpleLogger logger("");
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.log(LogLevel::Info, "Console message");
    
    std::string output = ss.str();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("Console message"), std::string::npos);
}

TEST(SimpleLoggerTest, AddOutputWorks) {
    SimpleLogger logger;
    logger.clearOutputs();
    
    logger.addOutput(std::make_unique<ConsoleOutput>());
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.log(LogLevel::Warning, "Added output test");
    
    std::string output = ss.str();
    EXPECT_NE(output.find("Added output test"), std::string::npos);
}

TEST(SimpleLoggerTest, ClearOutputsWorks) {
    SimpleLogger logger;
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.log(LogLevel::Info, "Before clear");
    EXPECT_FALSE(ss.str().empty());
    
    ss.str("");
    ss.clear();
    
    logger.clearOutputs();
    
    logger.log(LogLevel::Info, "After clear");
    EXPECT_TRUE(ss.str().empty());
}

TEST(SimpleLoggerTest, LogLevelFiltering) {
    SimpleLogger logger;
    logger.setLevel(LogLevel::Warning);
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());

    logger.log(LogLevel::Info, "Info message");
    EXPECT_TRUE(ss.str().empty());
    
    logger.log(LogLevel::Warning, "Warning message");
    EXPECT_NE(ss.str().find("Warning message"), std::string::npos);
}

TEST(SimpleLoggerTest, ILoggerInterfaceMethods) {
    SimpleLogger logger;
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger.info("Info test");
    logger.warning("Warning test");
    logger.error("Error test");
    
    std::string output = ss.str();
    EXPECT_NE(output.find("Info test"), std::string::npos);
    EXPECT_NE(output.find("Warning test"), std::string::npos);
    EXPECT_NE(output.find("Error test"), std::string::npos);
}