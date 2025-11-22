#pragma once
#include <string>
#include "FrameData.hpp"

namespace ds {
    class IImageExporter {
    public:
        virtual ~IImageExporter() = default;
        virtual bool exportFrame(const FrameData& frame, const std::string& path) const = 0;
    };
}