#include "VideoExporter.hpp"
#include "FFmpegCommandBuilder.hpp"
#include "SystemProcessManager.hpp"
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

namespace ds {

VideoExporter::VideoExporter() 
    : m_commandBuilder(std::make_unique<FFmpegCommandBuilder>()),
      m_processManager(std::make_unique<SystemProcessManager>()) {
}

VideoExporter::VideoExporter(std::unique_ptr<IVideoCommandBuilder> commandBuilder, 
                            std::unique_ptr<IProcessManager> processManager)
    : m_commandBuilder(std::move(commandBuilder)), 
      m_processManager(std::move(processManager)) {
}

std::unique_ptr<VideoExporter> VideoExporter::createDefault() {
    return std::make_unique<VideoExporter>();
}

bool VideoExporter::exportVideo(const std::vector<std::string>& frames, 
                                const VideoConfig& config, const std::string& output) {
    if (frames.empty()) {
        std::cerr << "[VideoExporter] Error: No frames provided\n";
        return false;
    }
    
    if (output.empty()) {
        std::cerr << "[VideoExporter] Error: No output path provided\n";
        return false;
    }
    
    std::filesystem::path firstFramePath(frames[0]);
    std::string framesDir = firstFramePath.parent_path().string();
    
    if (framesDir.empty()) {
        framesDir = ".";
    }
    
    std::string cmd = m_commandBuilder->buildCommand(framesDir, output, config);
    
    std::cout << "[VideoExporter] Executing: " << cmd << std::endl;
    
    bool success = m_processManager->execute(cmd, 
        [](const std::string& processOutput) {
            if (!processOutput.empty()) {
                std::cout << "[FFmpeg] " << processOutput << std::endl;
            }
        },
        []() { return false; },
        nullptr);
    
    if (!success) {
        std::cerr << "[VideoExporter] Error: Process failed\n";
        return false;
    }
    
    if (!std::filesystem::exists(output)) {
        std::cerr << "[VideoExporter] Error: Output file not created: " << output << std::endl;
        return false;
    }
    
    std::cout << "[VideoExporter] Success: Video exported to " << output << std::endl;
    return true;
}

bool VideoExporter::exportFromPNGs(const std::string& framesDir,
                                   const std::string& outputMp4,
                                   const VideoConfig& config,
                                   EventFn onEvent,
                                   CancelFn shouldCancel,
                                   int* outPid) {
    if (framesDir.empty() || outputMp4.empty()) {
        if (onEvent) onEvent({ExportEventType::Error,0,0,0.0,"Diretório ou arquivo vazio"});
        return false;
    }
    size_t pngCount = 0;
    for (auto &p : std::filesystem::directory_iterator(framesDir)) {
        if (p.is_regular_file()) {
            auto name = p.path().filename().string();
            if (name.rfind("frame_",0)==0 && p.path().extension()==".png") {
                ++pngCount;
            }
        }
    }
    if (pngCount == 0) {
        if (onEvent) onEvent({ExportEventType::Error,0,0,0.0,"Nenhum PNG encontrado"});
        return false;
    }
    if (onEvent) onEvent({ExportEventType::Start,0,pngCount,0.0,"Iniciando ffmpeg"});
    std::string cmd = m_commandBuilder->buildCommand(framesDir, outputMp4, config);
    if (onEvent) onEvent({ExportEventType::Progress,0,pngCount,0.0,"Exec: " + cmd});

    size_t lastFrameReported = 0;
    bool ok = m_processManager->execute(cmd,
        [&](const std::string& line){
            if (shouldCancel && shouldCancel()) {
                if (onEvent) onEvent({ExportEventType::Cancelled,lastFrameReported,pngCount,(double)lastFrameReported*100.0/pngCount,"Cancelado"});
                return;
            }
            auto pos = line.find("frame=");
            if (pos != std::string::npos) {
                size_t start = pos + 6;
                while (start < line.size() && line[start]==' ') ++start;
                size_t end = start;
                while (end < line.size() && isdigit(static_cast<unsigned char>(line[end]))) ++end;
                if (end > start) {
                    size_t frameNum = (size_t)std::stoul(line.substr(start,end-start));
                    lastFrameReported = frameNum;
                    double pct = pngCount>0? std::min(100.0,(double)frameNum*100.0/pngCount):0.0;
                    if (onEvent) onEvent({ExportEventType::Progress,frameNum,pngCount,pct,"frame="+std::to_string(frameNum)});
                }
            } else if (line.find("error") != std::string::npos || line.find("Invalid") != std::string::npos) {
                if (onEvent) onEvent({ExportEventType::Error,lastFrameReported,pngCount,(double)lastFrameReported*100.0/pngCount,line});
            }
        }, shouldCancel, outPid);
    if (!ok) {
        if (onEvent) onEvent({ExportEventType::Error,lastFrameReported,pngCount,(double)lastFrameReported*100.0/pngCount,"ffmpeg falhou"});
        return false;
    }
    if (!std::filesystem::exists(outputMp4)) {
        if (onEvent) onEvent({ExportEventType::Error,lastFrameReported,pngCount,(double)lastFrameReported*100.0/pngCount,"MP4 não criado"});
        return false;
    }
    if (onEvent) onEvent({ExportEventType::Completed,pngCount,pngCount,100.0,"Concluído"});
    return true;
}
} 