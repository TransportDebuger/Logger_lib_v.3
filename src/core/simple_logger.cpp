#include "logger/simple_logger.hpp"
#include "logger/log_level.hpp"
#include "logger/level_filter.hpp"
#include "logger/file_output.hpp"
#include "logger/console_output.hpp"
#include <stdexcept>
#include <iostream>

namespace stc {

SimpleLogger::SimpleLogger(std::unique_ptr<IFormatter> formatter)
    : formatter_(std::move(formatter)), min_level_(LogLevel::Debug) {
    if (!formatter_) {
        formatter_ = std::make_unique<StandardFormatter>();
    }
    addOutput(std::make_unique<ConsoleOutput>());
    addFilter(std::make_unique<LevelFilter>(min_level_));
}

SimpleLogger::SimpleLogger(const std::string& filename, std::unique_ptr<IFormatter> formatter)
    : formatter_(std::move(formatter)), min_level_(LogLevel::Debug) {
    if (!formatter_) {
        formatter_ = std::make_unique<StandardFormatter>();
    }
    if (!filename.empty()) {
        addOutput(std::make_unique<FileOutput>(filename));
    } else {
        addOutput(std::make_unique<ConsoleOutput>());
    }

    addFilter(std::make_unique<LevelFilter>(min_level_));
}

SimpleLogger::~SimpleLogger() = default;

void SimpleLogger::log(LogLevel level, const std::string& message) {
    LogMessage log_message(level, message);
    log(log_message);
}

void SimpleLogger::setLevel(LogLevel min_level) {
    std::lock_guard<std::mutex> lock(mutex_);
    min_level_ = min_level;
    
    if (!filters_.empty()) {
        auto* level_filter = dynamic_cast<LevelFilter*>(filters_[0].get());
        if (level_filter) {
            level_filter->setMinLevel(min_level);
        }
    }
}

LogLevel SimpleLogger::getLevel() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return min_level_;
}

bool SimpleLogger::isEnabled(LogLevel level) const {
    LogMessage test_message(level, "");
    return shouldProcessMessage(test_message);
}

void SimpleLogger::log(const LogMessage& log_message) {
    if (!shouldProcessMessage(log_message)) {
        return;
    }
    
    std::string formatted_message = formatter_->format(log_message);
    
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& output : outputs_) {
        output->write(formatted_message);
    }
}

void SimpleLogger::addFilter(std::unique_ptr<IFilter> filter) {
    std::lock_guard<std::mutex> lock(mutex_);
    filters_.push_back(std::move(filter));
}

void SimpleLogger::clearFilters() {
    std::lock_guard<std::mutex> lock(mutex_);
    filters_.clear();
    // Восстанавливаем базовый фильтр по уровню
    filters_.push_back(std::make_unique<LevelFilter>(min_level_));
}

size_t SimpleLogger::getFilterCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return filters_.size();
}

void SimpleLogger::addOutput(std::unique_ptr<IOutput> output) {
    std::lock_guard<std::mutex> lock(mutex_);
    outputs_.push_back(std::move(output));
}

void SimpleLogger::clearOutputs() {
    std::lock_guard<std::mutex> lock(mutex_);
    outputs_.clear();
}

void SimpleLogger::setFormatter(std::unique_ptr<IFormatter> formatter) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (formatter) {
        formatter_ = std::move(formatter);
    }
}

const IFormatter& SimpleLogger::getFormatter() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return *formatter_;
}

bool SimpleLogger::shouldProcessMessage(const LogMessage& message) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Применяем все фильтры в цепочке (Chain of Responsibility)
    for (const auto& filter : filters_) {
        if (!filter->shouldPass(message)) {
            return false;  // Если любой фильтр сказал "нет" - отклоняем
        }
    }
    
    return true;  // Все фильтры пропустили сообщение
}

} // namespace stc