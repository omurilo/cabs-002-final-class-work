#pragma once
#include "VisualizerBase.h"
#include "AnimationStrategy.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "../lib/include/datastructures.hpp"

class Visualizer : public VisualizerBase {
public:
    void setStrategy(std::unique_ptr<AnimationStrategy> s) { m_strategy = std::move(s); }
    void render(const std::vector<int>& state);
    void highlight(size_t index);
    void exportFrames(const std::string& dirPath, const std::string& prefix="frame");
    void exportFramesWithProgress(const std::string& dirPath, const std::string& prefix,
                                  const ds::PNGWriter::EventFn& onEvent,
                                  const ds::FrameStore::CancelFn& shouldCancel = nullptr);
    void exportAsMP4(const std::string& dirPath, const std::string& mp4File, const ds::VideoConfig& cfg = {});
    void exportAsMP4WithProgress(const std::string& dirPath, const std::string& mp4File, const ds::VideoConfig& cfg,
                                 const ds::VideoExporter::EventFn& onEvent,
                                 const ds::VideoExporter::CancelFn& shouldCancel = nullptr,
                                 int* outPid = nullptr);
    void captureFrame(const sf::RenderWindow& window);
    void refreshPositions(std::function<sf::Vector2f(size_t)> positionFn);
    bool saveFramesDAO(const std::string& dirPath);
    void clearMemoryFrames();
    bool clearSavedFrames(const std::string& dirPath);
    void toggleCapture() { m_frameStore.enable(!m_frameStore.enabled()); }
    bool isCaptureEnabled() const { return m_frameStore.enabled(); }
    void setCaptureMaxFrames(size_t maxFrames) { m_frameStore.setMax(maxFrames); }
    size_t getCapturedFrameCount() const { return m_frameStore.count(); }
    size_t getCaptureLimit() const { return m_frameStore.max(); }
    bool isIdle() const { return m_animationQueue.empty(); }
    void queueOperation(const std::string& description, std::function<void()> action) {
        enqueueOperation(description, std::move(action));
    }
protected:
    std::unique_ptr<AnimationStrategy> m_strategy = nullptr;
    ds::FrameStore m_frameStore{900};
};
