#pragma once
#include <atomic>
#include <string>
#include <thread>
#include <functional>
#include "datastructures.hpp" 
#include "Visualizer.h" 
#include "ExportStatusModel.h"


class ExportController {
public:
    using SubtitleFn = std::function<void(const std::string&)>;

    explicit ExportController(SubtitleFn push) : m_push(std::move(push)) {}
    
    void update(ExportStatusModel& statusModel) {
        
        const auto status = statusModel.getStatus();
        
        
        if (status.framesCancelRequested && !m_framesCancelRequested) {
            m_framesCancelRequested = true;
        }
        if (status.videoCancelRequested && !m_videoCancelRequested) {
            m_videoCancelRequested = true;
        }
    }

    void startPNG(ExportStatusModel& statusModel, Visualizer* viz, const std::string& dir, const std::string& prefix) {
        if (statusModel.getStatus().exportingFrames) return;
        
        statusModel.setFrameExport(true, 0, 0);
        m_push("Export PNG iniciada");
        m_framesCancelRequested = false;
        
        std::thread([this, &statusModel, viz, dir, prefix]{
            viz->exportFramesWithProgress(dir, prefix, [&statusModel](const ds::ExportEvent& ev){
                switch(ev.type) {
                    case ds::ExportEventType::Start: 
                        statusModel.setFrameExport(true, 0, ev.total); 
                        break;
                    case ds::ExportEventType::FrameSaved: 
                        statusModel.setFrameExport(true, ev.current, ev.total); 
                        break;
                    case ds::ExportEventType::Completed: 
                        statusModel.setFrameExport(false); 
                        break;
                    case ds::ExportEventType::Cancelled: 
                        statusModel.setFrameExport(false); 
                        break;
                    case ds::ExportEventType::Error: 
                        statusModel.setFrameExport(false); 
                        break;
                    default: break;
                }
            }, [this, &statusModel](){ return statusModel.isFrameCancelRequested() || m_framesCancelRequested; });
        }).detach();
    }

    void startMP4(ExportStatusModel& statusModel, Visualizer* viz, const std::string& frameDir, const std::string& outName, const ds::VideoConfig& cfg) {
        if (statusModel.getStatus().exportingVideo) return;
        
        statusModel.setVideoExport(true, "Iniciando...", false);
        m_push("Export MP4 iniciada");
        m_videoCancelRequested = false;
        
        std::thread([this, &statusModel, viz, frameDir, outName, cfg]{
            int videoPid = -1;
            viz->exportAsMP4WithProgress(frameDir, outName, cfg, [&statusModel](const ds::ExportEvent& ev){
                if (ev.type == ds::ExportEventType::Progress) {
                    statusModel.setVideoExport(true, ev.message, false);
                } else if (ev.type == ds::ExportEventType::Completed) {
                    statusModel.setVideoExport(false, "Concluido", true);
                } else if (ev.type == ds::ExportEventType::Cancelled) {
                    statusModel.setVideoExport(false, "Cancelado", true);
                } else if (ev.type == ds::ExportEventType::Error) {
                    statusModel.setVideoExport(false, "Erro", true);
                } else if (ev.type == ds::ExportEventType::Start) {
                    statusModel.setVideoExport(true, "Iniciando ffmpeg", false);
                }
            }, [this, &statusModel](){ return statusModel.isVideoCancelRequested() || m_videoCancelRequested; }, &videoPid);
            
            if (videoPid != -1) {
                statusModel.setVideoPid(videoPid);
            }
        }).detach();
    }

    void requestCancelVideo(ExportStatusModel& statusModel) { 
        statusModel.requestCancelVideo();
        m_push("Solicitado cancelamento video"); 
    }
    
    void requestCancelFrames(ExportStatusModel& statusModel) { 
        statusModel.requestCancelFrames();
        m_push("Solicitado cancelamento PNG"); 
    }

private:
    SubtitleFn m_push;
    std::atomic<bool> m_framesCancelRequested{false};
    std::atomic<bool> m_videoCancelRequested{false};
};
