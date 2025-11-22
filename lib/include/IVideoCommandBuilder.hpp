#pragma once
#include <string>
#include "VideoConfig.hpp"

namespace ds {
    class IVideoCommandBuilder {
    public:
        virtual ~IVideoCommandBuilder() = default;
        virtual std::string buildCommand(const std::string& framesDir, 
                                        const std::string& output, 
                                        const VideoConfig& config) const = 0;
    };
}