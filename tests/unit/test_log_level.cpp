#include <gtest/gtest.h>

#include "logger/log_level.hpp"

using namespace stc;

TEST(LogLevelTest, ToString) {
    EXPECT_EQ(logLevelToString(LogLevel::Debug), "DEBUG");
    EXPECT_EQ(logLevelToString(LogLevel::Info), "INFO");
    EXPECT_EQ(logLevelToString(LogLevel::Warning), "WARNING");
    EXPECT_EQ(logLevelToString(LogLevel::Error), "ERROR");
    EXPECT_EQ(logLevelToString(LogLevel::Fatal), "FATAL");
}

TEST(LogLevelTest, FromString) {
    EXPECT_EQ(stringToLogLevel("debug"), LogLevel::Debug);
    EXPECT_EQ(stringToLogLevel("INFO"), LogLevel::Info);
    EXPECT_EQ(stringToLogLevel("Warn"), LogLevel::Warning);
    EXPECT_EQ(stringToLogLevel("error"), LogLevel::Error);
    EXPECT_EQ(stringToLogLevel("FATAL"), LogLevel::Fatal);
    EXPECT_THROW(stringToLogLevel("unknown"), std::invalid_argument);
}

TEST(LogLevelTest, ShouldLog) {
    EXPECT_TRUE(shouldLog(LogLevel::Warning, LogLevel::Info));
    EXPECT_FALSE(shouldLog(LogLevel::Debug, LogLevel::Info));
    EXPECT_TRUE(shouldLog(LogLevel::Fatal, LogLevel::Fatal));
}