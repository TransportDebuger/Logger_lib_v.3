#include "logger/level_filter.hpp"

namespace stc {

LevelFilter::LevelFilter(LogLevel min_level) 
    : min_level_(min_level) {
}

bool LevelFilter::shouldPass(const LogMessage& message) const {
    return shouldLog(message.level, min_level_);
}

LogLevel LevelFilter::getMinLevel() const {
    return min_level_;
}

void LevelFilter::setMinLevel(LogLevel min_level) {
    min_level_ = min_level;
}

} // namespace stc