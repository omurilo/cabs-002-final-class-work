#pragma once
#include "VisualizerBase.h"
#include "AnimationStrategy.h"
#include "IView.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "datastructures.hpp"

class Visualizer : public VisualizerBase, public IView {
public:
    void setStrategy(std::unique_ptr<AnimationStrategy> s) { m_strategy = std::move(s); }
    void update(float dt) override { VisualizerBase::update(dt); }
    void syncState(const std::vector<int>& logical) override { render(logical); }
    void syncLabels(const std::vector<std::string>& labels) override {
        if (labels.size() == m_nodes.size()) {
            for (size_t i = 0; i < labels.size(); ++i) {
                m_nodes[i].label = labels[i];
            }
        }
    }
    void animateInsert(int value, size_t index) override;
    void animateInsertString(const std::string& value, size_t index) override { animateInsert(0, index); /* label aplicada via syncLabels */ }
    void animateRemove(size_t index) override;
    void animateHighlight(size_t index) override { highlight(index); }
    void animateClear() override;

    void render(const std::vector<int>& state);
    void highlight(size_t index);
    void exportFrames(const std::string& dirPath, const std::string& prefix="frame");
    void exportFramesWithProgress(const std::string& dirPath, const std::string& prefix,
                                  const ds::PNGWriter::EventFn& onEvent,
                                  const ds::FrameManager::CancelFn& shouldCancel = nullptr);
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
    void queueOperation(const std::string& description, std::function<void()> action) { enqueueOperation(description, std::move(action)); }
protected:
    std::unique_ptr<AnimationStrategy> m_strategy = nullptr;
    ds::FrameManager m_frameStore{900};
};

