#pragma once
#include <string>
#include <vector>
#include "VideoConfig.hpp"

namespace ds {
    class IVideoExporter {
    public:
        virtual ~IVideoExporter() = default;
        virtual bool exportVideo(const std::vector<std::string>& frames, 
                                const VideoConfig& config, const std::string& output) = 0;
    };
}