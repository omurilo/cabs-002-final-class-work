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

bool PNGWriter::save(const std::vector<RawImage>& frames,
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
        int stride = img.width * 4;
        int ok = stbi_write_png(filePath.c_str(), (int)img.width, (int)img.height, 4, img.pixels.data(), stride);
        if (!ok) {
            if (onEvent) onEvent({ExportEventType::Error,i,total,(double)i*100.0/total,"Falha ao salvar: "+filePath});
        }
        if (onEvent) onEvent({ExportEventType::FrameSaved,i+1,total,(double)(i+1)*100.0/total, filePath});
    }
    if (onEvent) onEvent({ExportEventType::Completed,total,total,100.0,"PNG salvo"});
    return true;
}

} // namespace ds