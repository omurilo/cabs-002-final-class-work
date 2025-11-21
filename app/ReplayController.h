#pragma once
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include "CommandRecorder.hpp" 

class ReplayController {
public:
    using RecordedCommand = ds::CommandRecorder::RecordedCommand;
    using ApplyFn = std::function<void(const RecordedCommand&)>; 

    void bindSource(const ds::CommandRecorder* recorder) { m_recorder = recorder; }
    void bindApply(ApplyFn fn) { m_apply = std::move(fn); }

    bool startTemporal() {
        if (!m_recorder) return false;
        m_index = 0; m_clock = 0.f; m_speed = 1.f; m_paused = false; m_active = true;
        return true;
    }
    void pauseToggle() { if (m_active) m_paused = !m_paused; }
    void step() {
        if (!m_active || !m_paused || !m_recorder) return;
        const auto& cmds = m_recorder->get();
        if (m_index < cmds.size()) {
            m_clock = cmds[m_index].t;
            m_paused = false; 
        }
    }
    void speedHalf() { m_speed = std::max(0.1f, m_speed * 0.5f); }
    void speedDouble() { m_speed = std::min(16.f, m_speed * 2.f); }

    void update(float dt) {
        if (!m_active || !m_recorder) return;
        if (!m_paused) m_clock += dt * m_speed;
        const auto& cmds = m_recorder->get();
        while (m_index < cmds.size() && cmds[m_index].t <= m_clock) {
            if (m_apply) m_apply(cmds[m_index]);
            ++m_index;
        }
        if (m_index >= cmds.size()) {
            m_active = false;
        }
    }

    bool active() const { return m_active; }
    bool paused() const { return m_paused; }
    float clock() const { return m_clock; }
    float speed() const { return m_speed; }
private:
    const ds::CommandRecorder* m_recorder = nullptr;
    ApplyFn m_apply;
    size_t m_index = 0;
    float m_clock = 0.f;
    float m_speed = 1.f;
    bool m_paused = false;
    bool m_active = false;
};
