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
        if (statusModel.getStatus().exportingFrames || m_shuttingDown) return;
        statusModel.setFrameExport(true, 0, 0);
        m_push("Export PNG iniciada");
        m_framesCancelRequested = false;
        m_threads.emplace_back([this, &statusModel, viz, dir, prefix]{
            viz->exportFramesWithProgress(dir, prefix, [&statusModel, this](const ds::ExportEvent& ev){
                if (m_shuttingDown) return;
                switch(ev.type) {
                    case ds::ExportEventType::Start: statusModel.setFrameExport(true, 0, ev.total); break;
                    case ds::ExportEventType::FrameSaved: statusModel.setFrameExport(true, ev.current, ev.total); break;
                    case ds::ExportEventType::Completed: statusModel.setFrameExport(false); break;
                    case ds::ExportEventType::Cancelled: statusModel.setFrameExport(false); break;
                    case ds::ExportEventType::Error: statusModel.setFrameExport(false); break;
                    default: break;
                }
            }, [this, &statusModel](){ return m_shuttingDown || statusModel.isFrameCancelRequested() || m_framesCancelRequested; });
        });
    }

    void startMP4(ExportStatusModel& statusModel, Visualizer* viz, const std::string& frameDir, const std::string& outName, const ds::VideoConfig& cfg) {
        if (statusModel.getStatus().exportingVideo || m_shuttingDown) return;
        statusModel.setVideoExport(true, "Iniciando...", false);
        m_push("Export MP4 iniciada");
        m_videoCancelRequested = false;
        m_threads.emplace_back([this, &statusModel, viz, frameDir, outName, cfg]{
            int videoPid = -1;
            viz->exportAsMP4WithProgress(frameDir, outName, cfg, [&statusModel, this](const ds::ExportEvent& ev){
                if (m_shuttingDown) return;
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
            }, [this, &statusModel](){ return m_shuttingDown || statusModel.isVideoCancelRequested() || m_videoCancelRequested; }, &videoPid);
            if (videoPid != -1 && !m_shuttingDown) {
                statusModel.setVideoPid(videoPid);
            }
        });
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
    std::atomic<bool> m_shuttingDown{false};
    std::vector<std::thread> m_threads;

public:
    void shutdown() {
        m_shuttingDown = true;
        m_framesCancelRequested = true;
        m_videoCancelRequested = true;
        for (auto &t : m_threads) {
            if (t.joinable()) t.join();
        }
        m_threads.clear();
    }
};