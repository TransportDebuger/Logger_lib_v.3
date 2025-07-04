#include "logger/simple_logger.hpp"

#include <iostream>
#include <stdexcept>

namespace stc {

SimpleLogger::SimpleLogger(const std::string& filename) {
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

void SimpleLogger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << message << std::endl;
}

void SimpleLogger::logToFile(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!file_stream_.is_open()) {
        throw std::runtime_error("Log file is not open");
    }
    file_stream_ << message << std::endl;
}

}  // namespace stc

/*! \example examples/basic_usage/main.cpp
 */