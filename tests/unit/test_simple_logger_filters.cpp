#include <gtest/gtest.h>
#include <sstream>
#include "logger/simple_logger.hpp"
#include "logger/level_filter.hpp"
#include "logger/component_filter.hpp"
#include "logger/console_output.hpp"

using namespace stc;

// Класс для перехвата cout
class CoutRedirect {
public:
    CoutRedirect(std::streambuf* new_buffer) : old(std::cout.rdbuf(new_buffer)) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
private:
    std::streambuf* old;
};

class SimpleLoggerFiltersTest : public ::testing::Test {
protected:
    void SetUp() override {
        logger_ = std::make_unique<SimpleLogger>();
        logger_->clearOutputs();
        logger_->addOutput(std::make_unique<ConsoleOutput>());
    }

    std::unique_ptr<SimpleLogger> logger_;
};

TEST_F(SimpleLoggerFiltersTest, DefaultFilterCountIsOne) {
    // По умолчанию должен быть LevelFilter
    EXPECT_EQ(logger_->getFilterCount(), 1);
}

TEST_F(SimpleLoggerFiltersTest, AddFilterIncreasesCount) {
    auto component_filter = std::make_unique<ComponentFilter>();
    logger_->addFilter(std::move(component_filter));
    
    EXPECT_EQ(logger_->getFilterCount(), 2);
}

TEST_F(SimpleLoggerFiltersTest, ClearFiltersResetsToDefaultLevelFilter) {
    // Добавляем несколько фильтров
    logger_->addFilter(std::make_unique<ComponentFilter>());
    logger_->addFilter(std::make_unique<LevelFilter>(LogLevel::Error));
    
    EXPECT_EQ(logger_->getFilterCount(), 3);
    
    // Очищаем фильтры
    logger_->clearFilters();
    
    // Должен остаться только базовый LevelFilter
    EXPECT_EQ(logger_->getFilterCount(), 1);
}

TEST_F(SimpleLoggerFiltersTest, LevelFilterWorks) {
    logger_->setLevel(LogLevel::Warning);
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    logger_->debug("Debug message");    // Не должно пройти
    logger_->info("Info message");      // Не должно пройти  
    logger_->warning("Warning message"); // Должно пройти
    logger_->error("Error message");    // Должно пройти
    
    std::string output = ss.str();
    
    EXPECT_EQ(output.find("Debug"), std::string::npos);
    EXPECT_EQ(output.find("Info"), std::string::npos);
    EXPECT_NE(output.find("Warning"), std::string::npos);
    EXPECT_NE(output.find("Error"), std::string::npos);
}

TEST_F(SimpleLoggerFiltersTest, ComponentFilterWorks) {
    // Добавляем фильтр компонентов (whitelist)
    auto component_filter = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Whitelist);
    component_filter->addComponent("Database");
    component_filter->addComponent("Network");
    logger_->addFilter(std::move(component_filter));
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    LogMessage db_msg(LogLevel::Info, "DB connected", "Database");
    LogMessage ui_msg(LogLevel::Info, "Button clicked", "UI");
    LogMessage net_msg(LogLevel::Info, "Request sent", "Network");
    
    logger_->log(db_msg);   // Должно пройти
    logger_->log(ui_msg);   // Не должно пройти
    logger_->log(net_msg);  // Должно пройти
    
    std::string output = ss.str();
    
    EXPECT_NE(output.find("DB connected"), std::string::npos);
    EXPECT_EQ(output.find("Button clicked"), std::string::npos);
    EXPECT_NE(output.find("Request sent"), std::string::npos);
}

TEST_F(SimpleLoggerFiltersTest, MultipleFiltersWorkInChain) {
    // Настраиваем фильтр уровня на Warning+
    logger_->setLevel(LogLevel::Warning);
    
    // Добавляем фильтр компонентов (только "Database")
    auto component_filter = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Whitelist);
    component_filter->addComponent("Database");
    logger_->addFilter(std::move(component_filter));
    
    std::stringstream ss;
    CoutRedirect redirect(ss.rdbuf());
    
    // Тестируем разные комбинации
    LogMessage db_info(LogLevel::Info, "DB info", "Database");        // Блок: низкий уровень
    LogMessage db_error(LogLevel::Error, "DB error", "Database");     // Пройдёт: высокий уровень + правильный компонент
    LogMessage ui_error(LogLevel::Error, "UI error", "UI");           // Блок: неправильный компонент
    LogMessage ui_info(LogLevel::Info, "UI info", "UI");              // Блок: низкий уровень + неправильный компонент
    
    logger_->log(db_info);
    logger_->log(db_error);
    logger_->log(ui_error);
    logger_->log(ui_info);
    
    std::string output = ss.str();
    
    // Только db_error должно пройти
    EXPECT_EQ(output.find("DB info"), std::string::npos);
    EXPECT_NE(output.find("DB error"), std::string::npos);
    EXPECT_EQ(output.find("UI error"), std::string::npos);
    EXPECT_EQ(output.find("UI info"), std::string::npos);
}

TEST_F(SimpleLoggerFiltersTest, IsEnabledRespectsFilters) {
    logger_->setLevel(LogLevel::Warning);  // Устанавливаем уровень Warning
    
    // Добавляем компонентный фильтр
    auto component_filter = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Whitelist);
    component_filter->addComponent("Database");
    logger_->addFilter(std::move(component_filter));
    
    // isEnabled создаёт тестовое сообщение с пустым компонентом "",
    // который не проходит ComponentFilter в режиме Whitelist
    EXPECT_FALSE(logger_->isEnabled(LogLevel::Debug));   // Блок: низкий уровень + пустой компонент
    EXPECT_FALSE(logger_->isEnabled(LogLevel::Error));   // Блок: пустой компонент (ComponentFilter)
    
    // Проверим с другим фильтром (Blacklist)
    logger_->clearFilters();  // Сбрасываем к базовому LevelFilter
    
    auto blacklist_filter = std::make_unique<ComponentFilter>(ComponentFilter::Mode::Blacklist);
    blacklist_filter->addComponent("Blocked");
    logger_->addFilter(std::move(blacklist_filter));
    
    // Blacklist с "Blocked" должен пропускать пустые компоненты
    EXPECT_FALSE(logger_->isEnabled(LogLevel::Debug));  // Блок: низкий уровень
    EXPECT_TRUE(logger_->isEnabled(LogLevel::Warning));  // Пройдёт: уровень OK + компонент не заблокирован
    EXPECT_TRUE(logger_->isEnabled(LogLevel::Error));   // Пройдёт: уровень OK + компонент не заблокирован
}