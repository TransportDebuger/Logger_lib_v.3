#include <gtest/gtest.h>

#include <regex>
#include <sstream>
#include <vector>

#include "logger/log_message.hpp"
#include "logger/standard_formatter.hpp"

using namespace stc;

TEST(StandardFormatterTest, DefaultPatternContainsAllFields) {
    StandardFormatter fmt;
    LogMessage msg(LogLevel::Info, "TestMessage", "Comp");
    std::string out = fmt.format(msg);

    // Должны присутствовать уровни, компонент, текст и метка времени
    EXPECT_NE(out.find("INFO"), std::string::npos);
    EXPECT_NE(out.find("Comp"), std::string::npos);
    EXPECT_NE(out.find("TestMessage"), std::string::npos);

    // Формат timestamp: YYYY-MM-DD HH:MM:SS.mmm
    std::regex time_regex(R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3}\])");
    EXPECT_TRUE(std::regex_search(out, time_regex));
}

TEST(StandardFormatterTest, CustomPatternReplacesPlaceholders) {
    std::string pattern = "{level}|{message}|{component}|{thread_id}";
    StandardFormatter fmt(pattern);

    LogMessage msg(LogLevel::Warning, "WarnMsg", "Mod");
    std::string out = fmt.format(msg);

    // Проверяем корректную подстановку
    auto parts = std::vector<std::string>();
    std::stringstream ss(out);
    std::string part;
    while (std::getline(ss, part, '|')) {
        parts.push_back(part);
    }

    ASSERT_EQ(parts.size(), 4u);
    EXPECT_EQ(parts[0], "WARNING");
    EXPECT_EQ(parts[1], "WarnMsg");
    EXPECT_EQ(parts[2], "Mod");
    EXPECT_FALSE(parts[3].empty());  // thread_id не пустой
}

TEST(StandardFormatterTest, ThreadIdFormatting) {
    StandardFormatter fmt("{thread_id}");
    LogMessage msg(LogLevel::Debug, "m", "");
    std::string out = fmt.format(msg);
    EXPECT_FALSE(out.empty());  // просто убедимся, что thread_id выводится
}