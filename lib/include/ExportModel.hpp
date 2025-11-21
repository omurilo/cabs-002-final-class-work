#pragma once
#include <memory>
#include "IImageExporter.hpp"
#include "IVideoExporter.hpp"

namespace ds {
    class ExportModel {
    private:
        std::unique_ptr<IImageExporter> m_imageExporter;
        std::unique_ptr<IVideoExporter> m_videoExporter;
        
    public:
        ExportModel();

        ExportModel(std::unique_ptr<IImageExporter> img, 
                   std::unique_ptr<IVideoExporter> vid);

        static std::unique_ptr<ExportModel> createDefault();

        bool exportFrame(const FrameData& frame, const std::string& path) {
            if (!m_imageExporter) return false;
            return m_imageExporter->exportFrame(frame, path);
        };

        bool exportVideo(const std::vector<std::string>& framePaths,
                        const VideoConfig& config, const std::string& outputPath) {
            if (!m_videoExporter) return false;
            return m_videoExporter->exportVideo(framePaths, config, outputPath);
        }

        void setImageExporter(std::unique_ptr<IImageExporter> exporter) {
            m_imageExporter = std::move(exporter);
        }
        void setVideoExporter(std::unique_ptr<IVideoExporter> exporter) {
            m_videoExporter = std::move(exporter);
        }
    };
}