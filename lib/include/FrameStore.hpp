#pragma once
#include "FrameData.hpp"
#include "ExportEvents.hpp"
#include "IImageExporter.hpp"
#include <vector>
#include <functional>
#include <memory>

namespace ds {

class FrameStore {
public:
    using CaptureFn = std::function<FrameData()>;
    using EventFn = std::function<void(const ExportEvent&)>;
    using CancelFn = std::function<bool()>;

    explicit FrameStore(size_t maxFrames = 900, std::unique_ptr<IImageExporter> exporter = nullptr) 
        : m_maxFrames(maxFrames), m_imageExporter(std::move(exporter)) {}
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
        FrameData img = fn();
        if (img.width == 0 || img.height == 0 || img.pixels.size() != img.width*img.height*4) return;
        if (m_frames.size() < m_maxFrames) {
            m_frames.push_back(std::move(img));
        } else if (m_circular) {
            m_frames[m_overwriteIndex] = std::move(img);
            m_overwriteIndex = (m_overwriteIndex + 1) % m_maxFrames;
        }
    }

    const std::vector<FrameData>& frames() const { return m_frames; }

    bool exportFrame(size_t index, const std::string& path) const {
        if (!m_imageExporter || index >= m_frames.size()) return false;
        return m_imageExporter->exportFrame(m_frames[index], path);
    }

    bool exportAllFrames(const std::string& basePath, const std::string& prefix = "frame_") const {
        if (!m_imageExporter) return false;
        bool allSuccess = true;
        for (size_t i = 0; i < m_frames.size(); ++i) {
            std::string path = basePath + "/" + prefix + std::to_string(i) + ".png";
            if (!m_imageExporter->exportFrame(m_frames[i], path)) {
                allSuccess = false;
            }
        }
        return allSuccess;
    }

    void setImageExporter(std::unique_ptr<IImageExporter> exporter) {
        m_imageExporter = std::move(exporter);
    }

private:
    size_t m_maxFrames;
    bool m_enabled = false;
    bool m_circular = false;
    size_t m_overwriteIndex = 0;
    std::vector<FrameData> m_frames;
    std::unique_ptr<IImageExporter> m_imageExporter;
};

}