#include "ExportModel.hpp"
#include "PNGWriter.hpp"
#include "VideoExporter.hpp"

namespace ds {

ExportModel::ExportModel()
    : m_imageExporter(std::make_unique<PNGWriter>()),
      m_videoExporter(std::make_unique<VideoExporter>()) {
}

ExportModel::ExportModel(std::unique_ptr<IImageExporter> img, 
                        std::unique_ptr<IVideoExporter> vid)
    : m_imageExporter(std::move(img)), 
      m_videoExporter(std::move(vid)) {
}

std::unique_ptr<ExportModel> ExportModel::createDefault() {
    return std::make_unique<ExportModel>();
}

} // namespace ds