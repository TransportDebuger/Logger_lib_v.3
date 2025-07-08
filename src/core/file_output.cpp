#include "logger/file_output.hpp"
#include <stdexcept>

namespace stc {

FileOutput::FileOutput(const std::string& filename) {
    file_stream_.open(filename, std::ios::app);
    if (!file_stream_.is_open()) {
        throw std::runtime_error("Cannot open log file: " + filename);
    }
}

FileOutput::~FileOutput() {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

void FileOutput::write(const std::string& formatted_message) {
    std::lock_guard<std::mutex> lock(file_mutex_);
    file_stream_ << formatted_message << std::endl;
    file_stream_.flush();
}

} // namespace stc