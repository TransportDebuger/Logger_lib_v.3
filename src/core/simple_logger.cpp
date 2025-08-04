#include "logger/simple_logger.hpp"
#include "logger/log_level.hpp"
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
}

SimpleLogger::~SimpleLogger() = default;

void SimpleLogger::log(LogLevel level, const std::string& message) {
    if (!isEnabled(level)) return;
    LogMessage log_message(level, message);
    log(log_message);
}

void SimpleLogger::setLevel(LogLevel min_level) {
    std::lock_guard<std::mutex> lock(mutex_);
    min_level_ = min_level;
}

LogLevel SimpleLogger::getLevel() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return min_level_;
}

bool SimpleLogger::isEnabled(LogLevel level) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return shouldLog(level, min_level_);
}

void SimpleLogger::log(const LogMessage& log_message) {
    if (!isEnabled(log_message.level)) return;
    std::string formatted_message = formatter_->format(log_message);
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& output : outputs_) {
        output->write(formatted_message);
    }
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

} // namespace stc