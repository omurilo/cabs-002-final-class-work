#include "ReplayEngine.hpp"
#include <algorithm>

namespace ds {

bool ReplayEngine::loadJSON(const std::string& path) {
    CommandRecorder temp; if (!temp.loadJSON(path)) return false;
    m_commands = temp.get();
    std::sort(m_commands.begin(), m_commands.end(), [](const RecordedCommand& a, const RecordedCommand& b){ return a.t < b.t; });
    reset();
    return true;
}

void ReplayEngine::reset() {
    m_cursor = 0; m_elapsed = 0.0; m_paused = false; }

void ReplayEngine::advance(double dt) {
    if (m_paused) return;
    m_elapsed += dt * m_speed;
    while (m_cursor < m_commands.size()) {
        const auto& c = m_commands[m_cursor];
        if (c.t <= m_elapsed) {
            if (c.op == "INSERT" && m_onInsert) m_onInsert(c);
            else if (c.op == "REMOVE" && m_onRemove) m_onRemove(c);
            else if (c.op == "HIGHLIGHT" && m_onHighlight) m_onHighlight(c);
            else if (c.op == "CLEAR" && m_onClear) m_onClear(c);
            ++m_cursor;
        } else {
            break;
        }
    }
}

} // namespace ds