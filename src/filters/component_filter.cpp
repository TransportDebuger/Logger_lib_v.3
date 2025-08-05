#include "logger/component_filter.hpp"

namespace stc {

ComponentFilter::ComponentFilter(Mode mode) 
    : mode_(mode) {
}

bool ComponentFilter::shouldPass(const LogMessage& message) const {
    bool found = components_.find(message.component) != components_.end();
    
    switch (mode_) {
        case Mode::Whitelist:
            return components_.empty() || found;  // Если список пуст, пропускаем всё
        case Mode::Blacklist:
            return !found;  // Блокируем только найденные
        default:
            return true;
    }
}

void ComponentFilter::addComponent(const std::string& component) {
    components_.insert(component);
}

void ComponentFilter::removeComponent(const std::string& component) {
    components_.erase(component);
}

void ComponentFilter::clearComponents() {
    components_.clear();
}

ComponentFilter::Mode ComponentFilter::getMode() const {
    return mode_;
}

} // namespace stc