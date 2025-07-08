#include "logger/simple_logger.hpp"

#include <iostream>
#include <stdexcept>

#include "logger/log_level.hpp"

namespace stc {

SimpleLogger::SimpleLogger(std::unique_ptr<IFormatter> formatter)
    : formatter_(std::move(formatter)), min_level_(LogLevel::Debug) {
    if (!formatter_) {
        formatter_ = std::make_unique<StandardFormatter>();
    }
}

SimpleLogger::SimpleLogger(const std::string& filename, std::unique_ptr<IFormatter> formatter)
    : formatter_(std::move(formatter)), min_level_(LogLevel::Debug) {
    if (!formatter_) {
        formatter_ = std::make_unique<StandardFormatter>();
    }

    if (!filename.empty()) {
        file_stream_.open(filename, std::ios::app);
        if (!file_stream_.is_open()) {
            throw std::runtime_error("Cannot open log file: " + filename);
        }
    }
}

SimpleLogger::~SimpleLogger() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

// Реализация интерфейса ILogger

void SimpleLogger::log(LogLevel level, const std::string& message) {
    if (!isEnabled(level)) {
        return;
    }

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
    return shouldLog(level, min_level_);
}

// Работа с LogMessage

void SimpleLogger::log(const LogMessage& log_message) {
    if (!isEnabled(log_message.level)) {
        return;
    }

    std::string formatted_message = formatter_->format(log_message);
    writeToConsole(formatted_message);

    if (file_stream_.is_open()) {
        writeToFile(formatted_message);
    }
}

void SimpleLogger::logToFile(const LogMessage& log_message) {
    if (!isEnabled(log_message.level)) {
        return;
    }

    if (!file_stream_.is_open()) {
        throw std::runtime_error("Log file is not open");
    }

    std::string formatted_message = formatter_->format(log_message);
    writeToFile(formatted_message);
}

// Методы для обратной совместимости

void SimpleLogger::log(const std::string& message) {
    log(LogLevel::Info, message);
}

void SimpleLogger::logToFile(const std::string& message) {
    LogMessage log_message(LogLevel::Info, message);
    logToFile(log_message);
}

// Управление форматтером

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

// Приватные методы

void SimpleLogger::writeToConsole(const std::string& formatted_message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << formatted_message << std::endl;
}

void SimpleLogger::writeToFile(const std::string& formatted_message) {
    std::lock_guard<std::mutex> lock(mutex_);
    file_stream_ << formatted_message << std::endl;
    file_stream_.flush();
}

}  // namespace stc