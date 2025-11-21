#include "PNGWriter.hpp"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace fs = std::filesystem;

namespace ds {

bool PNGWriter::exportFrame(const FrameData& frame, const std::string& path) const {
    if (frame.width == 0 || frame.height == 0 || frame.pixels.empty()) {
        return false;
    }

    std::error_code ec;
    auto parentPath = fs::path(path).parent_path();
    if (!parentPath.empty()) {
        fs::create_directories(parentPath, ec);
        if (ec) return false;
    }

    int stride = frame.width * 4;
    int ok = stbi_write_png(path.c_str(), (int)frame.width, (int)frame.height, 4, frame.pixels.data(), stride);
    return ok != 0;
}

bool PNGWriter::save(const std::vector<FrameData>& frames,
                     const std::string& dirPath,
                     const std::string& prefix,
                     EventFn onEvent,
                     CancelFn shouldCancel) const {
    if (frames.empty()) {
        if (onEvent) onEvent({ExportEventType::Error,0,0,0.0,"Nenhum frame"});
        return false;
    }
    std::error_code ec; fs::create_directories(dirPath, ec);
    if (ec) {
        if (onEvent) onEvent({ExportEventType::Error,0,0,0.0,"Falha criar diretorio"});
        return false;
    }
    size_t total = frames.size();
    if (onEvent) onEvent({ExportEventType::Start,0,total,0.0,"Iniciando salvamento"});
    for (size_t i=0;i<frames.size();++i) {
        if (shouldCancel && shouldCancel()) {
            if (onEvent) onEvent({ExportEventType::Cancelled,i,total,(double)i*100.0/total,"Cancelado"});
            return false;
        }
        const auto& img = frames[i];
        std::ostringstream fname; fname << prefix << '_' << std::setw(4) << std::setfill('0') << i << ".png";
        auto filePath = (fs::path(dirPath) / fname.str()).string();

        bool ok = exportFrame(img, filePath);
        if (!ok) {
            if (onEvent) onEvent({ExportEventType::Error,i,total,(double)i*100.0/total,"Falha ao salvar: "+filePath});
        }
        if (onEvent) onEvent({ExportEventType::FrameSaved,i+1,total,(double)(i+1)*100.0/total, filePath});
    }
    if (onEvent) onEvent({ExportEventType::Completed,total,total,100.0,"PNG salvo"});
    return true;
}

} // namespace ds
