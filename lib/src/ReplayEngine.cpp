#include "ReplayEngine.hpp"
#include "JSONCommandSerializer.hpp"
#include <algorithm>

namespace ds {

bool ReplayEngine::loadJSON(const std::string& path) {
    CommandRecorder temp; 
    if (!temp.load(path)) return false;
    m_commands = temp.getCommands();
    std::sort(m_commands.begin(), m_commands.end(), [](const CommandData& a, const CommandData& b){ return a.timestamp < b.timestamp; });
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
        double cmdTimeSec = c.timestamp.count() / 1000.0; 
        if (cmdTimeSec <= m_elapsed) {
            if ((c.operation == "INSERT" || c.operation == "INSERT BACK" || c.operation == "INSERT FRONT") && m_onInsert) {
                m_onInsert(c);
            }
            else if ((c.operation == "REMOVE" || c.operation == "REMOVE FRONT" || c.operation == "REMOVE BACK") && m_onRemove) {
                m_onRemove(c);
            }
            else if (c.operation == "HIGHLIGHT" && m_onHighlight) {
                m_onHighlight(c);
            }
            else if (c.operation == "CLEAR" && m_onClear) {
                m_onClear(c);
            }
            ++m_cursor;
        } else {
            break;
        }
    }
}

} 