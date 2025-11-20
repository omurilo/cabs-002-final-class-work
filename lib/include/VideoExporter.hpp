#pragma once
#include "VideoConfig.hpp"
#include "ExportEvents.hpp"
#include <string>
#include <functional>
#include <vector>
#include <cstdint>

namespace ds {

class VideoExporter {
public:
    using EventFn = std::function<void(const ExportEvent&)>;
    using CancelFn = std::function<bool()>;

    bool exportFromPNGs(const std::string& framesDir,
                        const std::string& outputFile,
                        const VideoConfig& cfg,
                        EventFn onEvent,
                        CancelFn shouldCancel,
                        int* outPid = nullptr) const;

    static bool cancelProcess(int pid);
};

}