#pragma once
#include "CommandRecorder.hpp"
#include <functional>
#include <string>

namespace ds {

class ReplayEngine {
public:
    using InsertCb = std::function<void(const RecordedCommand&)>;
    using RemoveCb = std::function<void(const RecordedCommand&)>;
    using HighlightCb = std::function<void(const RecordedCommand&)>;
    using ClearCb = std::function<void(const RecordedCommand&)>;

    bool loadJSON(const std::string& path);
    void reset();
    void setSpeed(double s) { m_speed = s; }
    void pause(bool p) { m_paused = p; }
    void advance(double dt);

    void onInsert(InsertCb cb) { m_onInsert = std::move(cb); }
    void onRemove(RemoveCb cb) { m_onRemove = std::move(cb); }
    void onHighlight(HighlightCb cb) { m_onHighlight = std::move(cb); }
    void onClear(ClearCb cb) { m_onClear = std::move(cb); }

private:
    std::vector<RecordedCommand> m_commands;
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