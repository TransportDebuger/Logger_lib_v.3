#include <gtest/gtest.h>
#include "logger/level_filter.hpp"
#include "logger/log_message.hpp"

using namespace stc;

class LevelFilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        filter_ = std::make_unique<LevelFilter>(LogLevel::Warning);
    }

    std::unique_ptr<LevelFilter> filter_;
};

TEST_F(LevelFilterTest, ConstructorSetsCorrectLevel) {
    EXPECT_EQ(filter_->getMinLevel(), LogLevel::Warning);
}

TEST_F(LevelFilterTest, ShouldPassHigherLevels) {
    LogMessage error_msg(LogLevel::Error, "Error message");
    LogMessage fatal_msg(LogLevel::Fatal, "Fatal message");
    
    EXPECT_TRUE(filter_->shouldPass(error_msg));
    EXPECT_TRUE(filter_->shouldPass(fatal_msg));
}

TEST_F(LevelFilterTest, ShouldPassSameLevel) {
    LogMessage warning_msg(LogLevel::Warning, "Warning message");
    
    EXPECT_TRUE(filter_->shouldPass(warning_msg));
}

TEST_F(LevelFilterTest, ShouldBlockLowerLevels) {
    LogMessage debug_msg(LogLevel::Debug, "Debug message");
    LogMessage info_msg(LogLevel::Info, "Info message");
    
    EXPECT_FALSE(filter_->shouldPass(debug_msg));
    EXPECT_FALSE(filter_->shouldPass(info_msg));
}

TEST_F(LevelFilterTest, SetMinLevelChangesFilterBehavior) {
    LogMessage info_msg(LogLevel::Info, "Info message");
    
    // Сначала блокируется
    EXPECT_FALSE(filter_->shouldPass(info_msg));
    
    // Меняем уровень
    filter_->setMinLevel(LogLevel::Debug);
    EXPECT_EQ(filter_->getMinLevel(), LogLevel::Debug);
    
    // Теперь пропускается
    EXPECT_TRUE(filter_->shouldPass(info_msg));
}

TEST_F(LevelFilterTest, WorksWithAllLogLevels) {
    // Тестируем все уровни
    std::vector<LogLevel> levels = {
        LogLevel::Debug, LogLevel::Info, LogLevel::Warning,
        LogLevel::Error, LogLevel::Fatal
    };
    
    for (auto level : levels) {
        LevelFilter test_filter(level);
        LogMessage msg(level, "Test message");
        
        EXPECT_TRUE(test_filter.shouldPass(msg));
        
        // Проверяем, что более высокие уровни проходят
        for (auto higher_level : levels) {
            if (static_cast<int>(higher_level) >= static_cast<int>(level)) {
                LogMessage higher_msg(higher_level, "Higher message");
                EXPECT_TRUE(test_filter.shouldPass(higher_msg));
            }
        }
    }
}