#pragma once
#include <string>
#include <functional>
#include <vector>
#include <mutex>

struct ExportStatus {
    bool exportingFrames = false;
    bool exportingVideo = false;
    size_t framesCurrent = 0;
    size_t framesTotal = 0;
    bool videoDone = false;
    bool framesCancelRequested = false;
    bool videoCancelRequested = false;
    int currentVideoPid = -1;
    std::string videoProgressLine;
    bool showCompletionMessage = false;
    std::string completionMessage;
    float completionTimeLeft = 0.0f;
};

class ExportStatusModel {
public:
    using ObserverFn = std::function<void(const ExportStatus&)>;
    
    void setFrameExport(bool exporting, size_t current = 0, size_t total = 0) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_status.exportingFrames = exporting;
            m_status.framesCurrent = current;
            m_status.framesTotal = total;
        }
        notify();
    }
    
    void setVideoExport(bool exporting, const std::string& progressLine = "", bool done = false) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_status.exportingVideo = exporting;
            m_status.videoProgressLine = progressLine;
            m_status.videoDone = done;
            
            
            if (done && !exporting && progressLine == "Concluido") {
                m_status.showCompletionMessage = true;
                m_status.completionMessage = "MP4 exportado com sucesso!";
                m_status.completionTimeLeft = 3.0f;
            }
        }
        notify();
    }
    
    void updateCompletionTimer(float deltaTime) {
        bool shouldNotify = false;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_status.showCompletionMessage) {
                m_status.completionTimeLeft -= deltaTime;
                if (m_status.completionTimeLeft <= 0.0f) {
                    m_status.showCompletionMessage = false;
                    m_status.completionMessage.clear();
                    shouldNotify = true;
                }
            }
        }
        if (shouldNotify) notify();
    }
    
    void requestCancelFrames() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_status.framesCancelRequested = true;
        }
        notify();
    }
    
    void requestCancelVideo() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_status.videoCancelRequested = true;
        }
        notify();
    }
    
    void setVideoPid(int pid) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_status.currentVideoPid = pid;
        }
        notify();
    }
    
    ExportStatus getStatus() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_status;
    }
    
    bool isFrameCancelRequested() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_status.framesCancelRequested;
    }
    
    bool isVideoCancelRequested() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_status.videoCancelRequested;
    }
    
    void attach(ObserverFn fn) { 
        std::lock_guard<std::mutex> lock(m_observerMutex);
        m_observers.push_back(fn); 
    }
    
private:
    void notify() {
        ExportStatus status;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            status = m_status;
        }
        std::lock_guard<std::mutex> lock(m_observerMutex);
        for (auto& fn : m_observers) {
            fn(status);
        }
    }
    
    mutable std::mutex m_mutex;
    mutable std::mutex m_observerMutex;
    ExportStatus m_status;
    std::vector<ObserverFn> m_observers;
};