#pragma once
#include <atomic>
#include <string>
#include <thread>
#include <functional>
#include "datastructures.hpp"
#include "Visualizer.h"

class ExportController {
public:
    struct Status {
        bool exportingFrames = false;
        bool exportingVideo = false;
        size_t framesCurrent = 0;
        size_t framesTotal = 0;
        bool videoDone = false;
        bool framesCancelRequested = false;
        bool videoCancelRequested = false;
        int currentVideoPid = -1;
        std::string videoProgressLine;
    };

    using SubtitleFn = std::function<void(const std::string&)>;

    explicit ExportController(SubtitleFn push) : m_push(std::move(push)) {}

    void startPNG(Visualizer* viz, const std::string& dir, const std::string& prefix) {
        if (m_status.exportingFrames) return;
        m_status.exportingFrames = true;
        m_status.framesCurrent = 0; m_status.framesTotal = 0; m_status.framesCancelRequested = false;
        m_push("Export PNG iniciada");
        std::thread([this, viz, dir, prefix]{
            viz->exportFramesWithProgress(dir, prefix, [&](const ds::ExportEvent& ev){
                switch(ev.type) {
                    case ds::ExportEventType::Start: m_status.framesCurrent=0; m_status.framesTotal=ev.total; break;
                    case ds::ExportEventType::FrameSaved: m_status.framesCurrent=ev.current; m_status.framesTotal=ev.total; break;
                    case ds::ExportEventType::Completed: m_push("PNG concluido"); m_status.exportingFrames=false; break;
                    case ds::ExportEventType::Cancelled: m_push("PNG cancelado"); m_status.exportingFrames=false; break;
                    case ds::ExportEventType::Error: m_push("Erro PNG"); break;
                    default: break;
                }
            }, [this](){ return m_status.framesCancelRequested; });
        }).detach();
    }

    void startMP4(Visualizer* viz, const std::string& frameDir, const std::string& outName, const ds::VideoConfig& cfg) {
        if (m_status.exportingVideo) return;
        m_status.exportingVideo = true; m_status.videoDone=false; m_status.videoCancelRequested=false; m_status.videoProgressLine.clear();
        m_push("Export MP4 iniciada");
        std::thread([this, viz, frameDir, outName, cfg]{
            viz->exportAsMP4WithProgress(frameDir, outName, cfg, [&](const ds::ExportEvent& ev){
                if (ev.type == ds::ExportEventType::Progress) {
                    m_status.videoProgressLine = ev.message;
                } else if (ev.type == ds::ExportEventType::Completed) {
                    m_push("MP4 concluido"); m_status.videoDone=true; m_status.exportingVideo=false; m_status.videoProgressLine = "Concluido";
                } else if (ev.type == ds::ExportEventType::Cancelled) {
                    m_push("MP4 cancelado"); m_status.videoDone=true; m_status.exportingVideo=false; m_status.videoProgressLine = "Cancelado";
                } else if (ev.type == ds::ExportEventType::Error) {
                    m_push("Erro MP4"); m_status.videoDone=true; m_status.exportingVideo=false; m_status.videoProgressLine = "Erro";
                } else if (ev.type == ds::ExportEventType::Start) {
                    m_status.videoProgressLine = "Iniciando ffmpeg";
                }
            }, [this](){ return m_status.videoCancelRequested; }, &m_status.currentVideoPid);
        }).detach();
    }

    void requestCancelVideo() { if (m_status.exportingVideo && !m_status.videoDone) { m_status.videoCancelRequested=true; m_push("Solicitado cancelamento video"); } }
    void requestCancelFrames() { if (m_status.exportingFrames) { m_status.framesCancelRequested=true; m_push("Solicitado cancelamento PNG"); } }

    const Status& status() const { return m_status; }
private:
    Status m_status;
    SubtitleFn m_push;
};
