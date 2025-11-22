#pragma once
#include "VideoConfig.hpp"
#include "ExportEvents.hpp"
#include <string>
#include <functional>
#include <vector>
#include <cstdint>
#include <memory>
#include "IVideoExporter.hpp"
#include "IVideoCommandBuilder.hpp" 
#include "IProcessManager.hpp"

namespace ds {
    class VideoExporter : public IVideoExporter {
    private:
        std::unique_ptr<IVideoCommandBuilder> m_commandBuilder;
        std::unique_ptr<IProcessManager> m_processManager;
    public:
        using EventFn = std::function<void(const ExportEvent&)>; 
        using CancelFn = std::function<bool()>; 
        
        VideoExporter();
        
        
        VideoExporter(std::unique_ptr<IVideoCommandBuilder> commandBuilder, 
                     std::unique_ptr<IProcessManager> processManager);

        
        static std::unique_ptr<VideoExporter> createDefault();

        bool exportVideo(const std::vector<std::string>& frames, 
                        const VideoConfig& config, const std::string& output) override;

        
        
        bool exportFromPNGs(const std::string& framesDir,
                            const std::string& outputMp4,
                            const VideoConfig& config,
                            EventFn onEvent,
                            CancelFn shouldCancel,
                            int* outPid);
    };
}
