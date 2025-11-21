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
    if (onEvent) onEvent(ExportEvent{ExportEventType::Error, 0, 0, 0.0, "Diretorio ou arquivo de saída vazio"});
        return false;
    }
    if (onEvent) onEvent(ExportEvent{ExportEventType::Start, 0, 0, 0.0, "Iniciando export"});
    std::string cmd = m_commandBuilder->buildCommand(framesDir, outputMp4, config);
    if (onEvent) onEvent(ExportEvent{ExportEventType::Progress, 0, 0, 0.0, "Chamada ffmpeg"});
    bool ok = m_processManager->execute(cmd,
        [&](const std::string& line){
            if (shouldCancel && shouldCancel()) {
                if (onEvent) onEvent(ExportEvent{ExportEventType::Cancelled, 0, 0, 0.0, "Cancelado"});
                return;
            }
            if (line.find("frame=") != std::string::npos) {
                if (onEvent) onEvent(ExportEvent{ExportEventType::Progress, 0, 0, 0.0, line});
            }
        }, shouldCancel, outPid);
    if (!ok) {
    if (onEvent) onEvent(ExportEvent{ExportEventType::Error, 0, 0, 0.0, "ffmpeg falhou"});
        return false;
    }
    if (!std::filesystem::exists(outputMp4)) {
    if (onEvent) onEvent(ExportEvent{ExportEventType::Error, 0, 0, 0.0, "Arquivo não criado"});
        return false;
    }
    if (onEvent) onEvent(ExportEvent{ExportEventType::Completed, 0, 0, 0.0, "Concluido"});
    return true;
}
} 
