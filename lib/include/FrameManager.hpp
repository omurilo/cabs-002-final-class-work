#pragma once
#include "FrameData.hpp"
#include "ExportEvents.hpp"
#include "IImageExporter.hpp"
#include "FrameRepository.hpp"
#include <vector>
#include <functional>
#include <memory>

namespace ds {

class FrameManager {
public:
    using CaptureFn = std::function<FrameData()>; 
    using EventFn = std::function<void(const ExportEvent&)>;
    using CancelFn = std::function<bool()>;

    explicit FrameManager(size_t maxFrames = 900, std::unique_ptr<IImageExporter> exporter = nullptr) 
        : m_repository(maxFrames), m_imageExporter(std::move(exporter)) {}
    
    
    void enable(bool on) { m_enabled = on; }
    bool enabled() const { return m_enabled; }
    size_t count() const { return m_repository.count(); }
    void clear() { m_repository.clear(); }
    void setCircular(bool c) { m_repository.setCircular(c); }
    bool circular() const { return m_repository.isCircular(); }
    void setMax(size_t m) { m_repository.setMaxFrames(m); }
    size_t max() const { return m_repository.maxFrames(); }

    void capture(CaptureFn fn) {
        if (!m_enabled) return;
        FrameData img = fn();
        if (img.width == 0 || img.height == 0 || img.pixels.size() != img.width*img.height*4) return;
        m_repository.store(std::move(img));
    }

    const std::vector<FrameData>& frames() const { return m_repository.getAll(); }

    
    bool exportFrame(size_t index, const std::string& path) const {
        if (!m_imageExporter || index >= m_repository.count()) return false;
        return m_imageExporter->exportFrame(m_repository.get(index), path);
    }

    
    bool exportAllFrames(const std::string& basePath, const std::string& prefix = "frame_") const {
        if (!m_imageExporter) return false;
        bool allSuccess = true;
        const auto& allFrames = m_repository.getAll();
        for (size_t i = 0; i < allFrames.size(); ++i) {
            std::string path = basePath + "/" + prefix + std::to_string(i) + ".png";
            if (!m_imageExporter->exportFrame(allFrames[i], path)) {
                allSuccess = false;
            }
        }
        return allSuccess;
    }

    
    void setImageExporter(std::unique_ptr<IImageExporter> exporter) {
        m_imageExporter = std::move(exporter);
    }

private:
    FrameRepository m_repository; 
    std::unique_ptr<IImageExporter> m_imageExporter; 
    bool m_enabled = false;
};

} 