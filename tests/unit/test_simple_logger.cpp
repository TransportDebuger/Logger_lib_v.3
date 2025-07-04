#include <gtest/gtest.h>
#include "logger/simple_logger.hpp"
#include <fstream>

using namespace stc_logger;

TEST(SimpleLoggerTest, ConsoleLog) {
    SimpleLogger logger;
    EXPECT_NO_THROW(logger.log("Console test"));
}

TEST(SimpleLoggerTest, FileLog) {
    const std::string fname = "test.log";
    SimpleLogger logger(fname);
    EXPECT_NO_THROW(logger.logToFile("File test"));
    std::ifstream f(fname);
    std::string line;
    std::getline(f, line);
    EXPECT_NE(line.find("File test"), std::string::npos);
}