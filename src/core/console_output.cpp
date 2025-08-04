#include "logger/console_output.hpp"

namespace stc {

std::mutex ConsoleOutput::cout_mutex_;

void ConsoleOutput::write(const std::string& formatted_message) {
    std::lock_guard<std::mutex> lock(cout_mutex_);
    std::cout << formatted_message << std::endl;
}

} // namespace stc