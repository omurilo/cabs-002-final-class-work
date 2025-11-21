#pragma once
#include "ReplayData.hpp"
#include "CommandRecorder.hpp"
#include "JSONCommandSerializer.hpp"
#include <functional>
#include <string>
#include <memory>

namespace ds {

class ReplayManager {
public:
    using InsertCb = std::function<void(const CommandData&)>;
    using RemoveCb = std::function<void(const CommandData&)>;
    using HighlightCb = std::function<void(const CommandData&)>;
    using ClearCb = std::function<void(const CommandData&)>;

    ReplayManager() = default;

    bool loadJSON(const std::string& path) {
        auto serializer = std::make_unique<JSONCommandSerializer>();
        CommandRecorder temp(std::move(serializer));
        if (!temp.load(path)) return false;

        m_data.commands = temp.getCommands();
        m_data.sortByTimestamp();
        m_data.calculateDuration();
        
        reset();
        return true;
    }
    
    void reset() {
        m_cursor = 0; 
        m_elapsed = 0.0; 
        m_paused = false; 
    }
    
    void setSpeed(double s) { m_speed = s; }
    void pause(bool p) { m_paused = p; }
    
    void advance(double dt) {
        if (m_paused) return;
        m_elapsed += dt * m_speed;
        
        while (m_cursor < m_data.commands.size()) {
            const auto& c = m_data.commands[m_cursor];
            if (c.timestamp.count() <= m_elapsed) {
                if (c.operation == "INSERT" && m_onInsert) m_onInsert(c);
                else if (c.operation == "REMOVE" && m_onRemove) m_onRemove(c);
                else if (c.operation == "HIGHLIGHT" && m_onHighlight) m_onHighlight(c);
                else if (c.operation == "CLEAR" && m_onClear) m_onClear(c);
                ++m_cursor;
            } else {
                break;
            }
        }
    }

    void onInsert(InsertCb cb) { m_onInsert = std::move(cb); }
    void onRemove(RemoveCb cb) { m_onRemove = std::move(cb); }
    void onHighlight(HighlightCb cb) { m_onHighlight = std::move(cb); }
    void onClear(ClearCb cb) { m_onClear = std::move(cb); }

    const ReplayData& getData() const { return m_data; }
    size_t getCurrentIndex() const { return m_cursor; }
    double getElapsedTime() const { return m_elapsed; }
    double getSpeed() const { return m_speed; }
    bool isPaused() const { return m_paused; }

private:
    ReplayData m_data;
    size_t m_cursor = 0;
    double m_elapsed = 0.0;
    double m_speed = 1.0;
    bool m_paused = false;

    InsertCb m_onInsert;
    RemoveCb m_onRemove;
    HighlightCb m_onHighlight;
    ClearCb m_onClear;
};

} // namespace ds