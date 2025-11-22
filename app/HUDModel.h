#pragma once
#include <functional>
#include <vector>
#include <string>

struct HUDState {
    bool recording = false;
    bool capturing = false;
    bool showLimitStatus = false;
    size_t captureLimit = 0;
    bool replayActive = false;
    float replayClock = 0.f;
    bool replayPaused = false;
    float replaySpeed = 1.f;
    bool exportingFrames = false;
    bool exportingVideo = false;
    size_t framesCurrent = 0;
    size_t framesTotal = 0;
    std::string videoProgressLine;
    bool showCompletionMessage = false;
    std::string completionMessage;
};

class HUDModel {
public:
    using ObserverFn = std::function<void(const HUDState&)>;
    
    void setRecording(bool recording) {
        if (m_state.recording != recording) {
            m_state.recording = recording;
            notify();
        }
    }
    
    void setCapturing(bool capturing) {
        if (m_state.capturing != capturing) {
            m_state.capturing = capturing;
            notify();
        }
    }
    
    void setCaptureLimit(size_t limit, bool show = false) {
        if (m_state.captureLimit != limit || m_state.showLimitStatus != show) {
            m_state.captureLimit = limit;
            m_state.showLimitStatus = show;
            notify();
        }
    }
    
    void setReplayState(bool active, float clock = 0.f, bool paused = false, float speed = 1.f) {
        if (m_state.replayActive != active || m_state.replayClock != clock || 
            m_state.replayPaused != paused || m_state.replaySpeed != speed) {
            m_state.replayActive = active;
            m_state.replayClock = clock;
            m_state.replayPaused = paused;
            m_state.replaySpeed = speed;
            notify();
        }
    }
    
    void setExportFrames(bool exporting, size_t current = 0, size_t total = 0) {
        if (m_state.exportingFrames != exporting || m_state.framesCurrent != current || m_state.framesTotal != total) {
            m_state.exportingFrames = exporting;
            m_state.framesCurrent = current;
            m_state.framesTotal = total;
            notify();
        }
    }
    
    void setExportVideo(bool exporting, const std::string& progress = "") {
        if (m_state.exportingVideo != exporting || m_state.videoProgressLine != progress) {
            m_state.exportingVideo = exporting;
            m_state.videoProgressLine = progress;
            notify();
        }
    }
    
    void setCompletionMessage(bool show, const std::string& message = "") {
        if (m_state.showCompletionMessage != show || m_state.completionMessage != message) {
            m_state.showCompletionMessage = show;
            m_state.completionMessage = message;
            notify();
        }
    }
    
    void hideLimitStatus() {
        if (m_state.showLimitStatus) {
            m_state.showLimitStatus = false;
            notify();
        }
    }
    
    const HUDState& getState() const { return m_state; }
    
    void attach(ObserverFn fn) { m_observers.push_back(fn); }

    void detachAll() { m_observers.clear(); }
    
private:
    void notify() {
        for (auto& fn : m_observers) {
            fn(m_state);
        }
    }
    
    HUDState m_state;
    std::vector<ObserverFn> m_observers;
};