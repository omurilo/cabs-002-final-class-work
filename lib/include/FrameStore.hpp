#pragma once
#include "RawImage.hpp"
#include "ExportEvents.hpp"
#include <vector>
#include <functional>

namespace ds {

class FrameStore {
public:
    using CaptureFn = std::function<RawImage()>; // fornecido pelo app
    using EventFn = std::function<void(const ExportEvent&)>;
    using CancelFn = std::function<bool()>;

    explicit FrameStore(size_t maxFrames = 900) : m_maxFrames(maxFrames) {}
    void enable(bool on) { m_enabled = on; }
    bool enabled() const { return m_enabled; }
    size_t count() const { return m_frames.size(); }
    void clear() { m_frames.clear(); m_overwriteIndex = 0; }
    void setCircular(bool c) { m_circular = c; }
    bool circular() const { return m_circular; }
    void setMax(size_t m) {
        if (m == 0) return;
        if (m < m_frames.size()) m_frames.resize(m);
        m_maxFrames = m;
        if (m_overwriteIndex >= m_maxFrames) m_overwriteIndex = 0;
    }
    size_t max() const { return m_maxFrames; }

    void capture(CaptureFn fn) {
        if (!m_enabled) return;
        RawImage img = fn();
        if (img.width == 0 || img.height == 0 || img.pixels.size() != img.width*img.height*4) return;
        if (m_frames.size() < m_maxFrames) {
            m_frames.push_back(std::move(img));
        } else if (m_circular) {
            m_frames[m_overwriteIndex] = std::move(img);
            m_overwriteIndex = (m_overwriteIndex + 1) % m_maxFrames;
        }
    }

    const std::vector<RawImage>& frames() const { return m_frames; }

private:
    size_t m_maxFrames;
    bool m_enabled = false;
    bool m_circular = false;
    size_t m_overwriteIndex = 0;
    std::vector<RawImage> m_frames;
};

}