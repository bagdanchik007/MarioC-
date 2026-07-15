#include "../include/EventBus.h"

void EventBus::subscribe(EventType type, Handler handler) {
    m_handlers[type].push_back(std::move(handler));
}

void EventBus::publish(const GameEvent& event) const {
    const auto it = m_handlers.find(event.type);
    if (it == m_handlers.end()) {
        return; // Niemand hoert auf diesen Ereignistyp - kein Fehler, einfach ignorieren.
    }
    for (const auto& handler : it->second) {
        handler(event);
    }
}
