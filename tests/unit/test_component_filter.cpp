#include <gtest/gtest.h>
#include "logger/component_filter.hpp"
#include "logger/log_message.hpp"

using namespace stc;

class ComponentFilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        whitelist_filter_ = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Whitelist);
        blacklist_filter_ = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Blacklist);
    }

    std::unique_ptr<ComponentFilter> whitelist_filter_;
    std::unique_ptr<ComponentFilter> blacklist_filter_;
};

// Тесты для Whitelist режима
TEST_F(ComponentFilterTest, WhitelistEmptyListPassesAll) {
    LogMessage db_msg(LogLevel::Info, "DB message", "Database");
    LogMessage ui_msg(LogLevel::Info, "UI message", "UI");
    
    // Пустой whitelist пропускает всё
    EXPECT_TRUE(whitelist_filter_->shouldPass(db_msg));
    EXPECT_TRUE(whitelist_filter_->shouldPass(ui_msg));
}

TEST_F(ComponentFilterTest, WhitelistPassesOnlyAllowedComponents) {
    whitelist_filter_->addComponent("Database");
    whitelist_filter_->addComponent("Network");
    
    LogMessage db_msg(LogLevel::Info, "DB message", "Database");
    LogMessage net_msg(LogLevel::Info, "Network message", "Network");
    LogMessage ui_msg(LogLevel::Info, "UI message", "UI");
    
    EXPECT_TRUE(whitelist_filter_->shouldPass(db_msg));
    EXPECT_TRUE(whitelist_filter_->shouldPass(net_msg));
    EXPECT_FALSE(whitelist_filter_->shouldPass(ui_msg));
}

// Тесты для Blacklist режима
TEST_F(ComponentFilterTest, BlacklistEmptyListPassesAll) {
    LogMessage db_msg(LogLevel::Info, "DB message", "Database");
    LogMessage ui_msg(LogLevel::Info, "UI message", "UI");
    
    // Пустой blacklist пропускает всё
    EXPECT_TRUE(blacklist_filter_->shouldPass(db_msg));
    EXPECT_TRUE(blacklist_filter_->shouldPass(ui_msg));
}

TEST_F(ComponentFilterTest, BlacklistBlocksOnlyForbiddenComponents) {
    blacklist_filter_->addComponent("Debug");
    blacklist_filter_->addComponent("Temp");
    
    LogMessage db_msg(LogLevel::Info, "DB message", "Database");
    LogMessage debug_msg(LogLevel::Info, "Debug message", "Debug");
    LogMessage temp_msg(LogLevel::Info, "Temp message", "Temp");
    
    EXPECT_TRUE(blacklist_filter_->shouldPass(db_msg));
    EXPECT_FALSE(blacklist_filter_->shouldPass(debug_msg));
    EXPECT_FALSE(blacklist_filter_->shouldPass(temp_msg));
}

// Тесты управления компонентами
TEST_F(ComponentFilterTest, AddRemoveComponentsWorks) {
    whitelist_filter_->addComponent("TestComponent");
    
    LogMessage test_msg(LogLevel::Info, "Test message", "TestComponent");
    LogMessage other_msg(LogLevel::Info, "Other message", "Other");
    
    EXPECT_TRUE(whitelist_filter_->shouldPass(test_msg));
    EXPECT_FALSE(whitelist_filter_->shouldPass(other_msg));
    
    // Удаляем компонент
    whitelist_filter_->removeComponent("TestComponent");
    
    EXPECT_TRUE(whitelist_filter_->shouldPass(test_msg));  // Теперь пропускает (список пуст)
    EXPECT_TRUE(whitelist_filter_->shouldPass(other_msg));
}

TEST_F(ComponentFilterTest, ClearComponentsWorks) {
    whitelist_filter_->addComponent("Comp1");
    whitelist_filter_->addComponent("Comp2");
    
    LogMessage comp1_msg(LogLevel::Info, "Message", "Comp1");
    LogMessage comp3_msg(LogLevel::Info, "Message", "Comp3");
    
    EXPECT_TRUE(whitelist_filter_->shouldPass(comp1_msg));
    EXPECT_FALSE(whitelist_filter_->shouldPass(comp3_msg));
    
    whitelist_filter_->clearComponents();
    
    // После очистки всё должно проходить
    EXPECT_TRUE(whitelist_filter_->shouldPass(comp1_msg));
    EXPECT_TRUE(whitelist_filter_->shouldPass(comp3_msg));
}

TEST_F(ComponentFilterTest, GetModeReturnsCorrectMode) {
    EXPECT_EQ(whitelist_filter_->getMode(), ComponentFilter::Mode::Whitelist);
    EXPECT_EQ(blacklist_filter_->getMode(), ComponentFilter::Mode::Blacklist);
}