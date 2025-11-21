#pragma once
#include <vector>
#include <functional>
#include "CommandRecorder.hpp"

struct ReplayState {
    bool active = false;
    bool paused = false;
    size_t currentIndex = 0;
    float clock = 0.f;
    float speed = 1.f;
    bool hasCommands = false;
    size_t totalCommands = 0;
};

class ReplayModel {
public:
    using ObserverFn = std::function<void(const ReplayState&)>;
    using RecordedCommand = ds::CommandRecorder::RecordedCommand;
    
    void bindSource(const ds::CommandRecorder* recorder) { 
        m_recorder = recorder;
        updateCommandCount();
    }
    
    bool startTemporal() {
        if (!m_recorder) return false;
        m_state.active = true;
        m_state.paused = false;
        m_state.currentIndex = 0;
        m_state.clock = 0.f;
        m_state.speed = 1.f;
        updateCommandCount();
        notify();
        return true;
    }
    
    void stop() {
        if (m_state.active) {
            m_state.active = false;
            notify();
        }
    }
    
    void pauseToggle() {
        if (m_state.active) {
            m_state.paused = !m_state.paused;
            notify();
        }
    }
    
    void step() {
        if (!m_state.active || !m_state.paused || !m_recorder) return;
        const auto& cmds = m_recorder->get();
        if (m_state.currentIndex < cmds.size()) {
            m_state.clock = cmds[m_state.currentIndex].t;
            m_state.paused = false;
            notify();
        }
    }
    
    void setSpeed(float speed) {
        speed = std::max(0.1f, std::min(16.f, speed));
        if (m_state.speed != speed) {
            m_state.speed = speed;
            notify();
        }
    }
    
    void speedHalf() { setSpeed(m_state.speed * 0.5f); }
    void speedDouble() { setSpeed(m_state.speed * 2.f); }
    
    void advanceClock(float dt) {
        if (m_state.active && !m_state.paused) {
            m_state.clock += dt * m_state.speed;
            
            if (m_recorder) {
                const auto& cmds = m_recorder->get();
                if (m_state.currentIndex >= cmds.size()) {
                    m_state.active = false;
                }
            }
            notify();
        }
    }
    
    std::vector<RecordedCommand> getCommandsToExecute() {
        std::vector<RecordedCommand> commands;
        if (!m_state.active || !m_recorder) return commands;
        
        const auto& allCommands = m_recorder->get();
        while (m_state.currentIndex < allCommands.size() && 
               allCommands[m_state.currentIndex].t <= m_state.clock) {
            commands.push_back(allCommands[m_state.currentIndex]);
            ++m_state.currentIndex;
        }
        
        if (!commands.empty()) notify();
        return commands;
    }
    
    const ReplayState& getState() const { return m_state; }
    
    void attach(ObserverFn fn) { m_observers.push_back(fn); }
    
private:
    void updateCommandCount() {
        if (m_recorder) {
            m_state.totalCommands = m_recorder->get().size();
            m_state.hasCommands = m_state.totalCommands > 0;
        } else {
            m_state.totalCommands = 0;
            m_state.hasCommands = false;
        }
    }
    
    void notify() {
        for (auto& fn : m_observers) {
            fn(m_state);
        }
    }
    
    ReplayState m_state;
    const ds::CommandRecorder* m_recorder = nullptr;
    std::vector<ObserverFn> m_observers;
};