#include "Visualizer.h"
#include <filesystem>
#include <iostream>

void Visualizer::render(const std::vector<int>& state) {
    if (m_nodes.size() == state.size()) {
        for (size_t i = 0; i < state.size(); ++i) m_nodes[i].value = state[i];
    }
}

void Visualizer::highlight(size_t index) {
    if (index < m_nodes.size()) {
        enqueueAnimation(std::make_unique<ColorStep>(index, sf::Color::Magenta, 0.25f));
        enqueueAnimation(std::make_unique<ColorStep>(index, sf::Color::Cyan, 0.25f));
    }
}

void Visualizer::exportFrames(const std::string& dirPath, const std::string& prefix) {
    if (m_frameStore.enabled()) {
        clearSavedFrames(dirPath);
    }
    ds::PNGWriter writer;
    writer.save(m_frameStore.frames(), dirPath, prefix,
        [](const ds::ExportEvent& ev){
            if (ev.type == ds::ExportEventType::Error) std::cerr << "[PNG Export] " << ev.message << '\n';
        }, nullptr);
}

void Visualizer::exportFramesWithProgress(const std::string& dirPath, const std::string& prefix,
                                          const ds::PNGWriter::EventFn& onEvent,
                                          const ds::FrameStore::CancelFn& shouldCancel) {
    if (m_frameStore.enabled()) clearSavedFrames(dirPath);
    ds::PNGWriter writer;
    writer.save(m_frameStore.frames(), dirPath, prefix, onEvent, shouldCancel);
}

void Visualizer::captureFrame(const sf::RenderWindow& window) {
    if (!m_frameStore.enabled()) return;
    auto size = window.getSize();
    sf::Texture tex; tex.create(size.x, size.y); tex.update(window);
    sf::Image img = tex.copyToImage();
    ds::RawImage raw; raw.width = size.x; raw.height = size.y;
    raw.pixels.assign(img.getPixelsPtr(), img.getPixelsPtr() + raw.width*raw.height*4);
    if (raw.pixels.size() == raw.width*raw.height*4) {
        auto& frames = const_cast<std::vector<ds::RawImage>&>(m_frameStore.frames());
        if (frames.size() < m_frameStore.max()) {
            frames.push_back(std::move(raw));
        }
    }
}

void Visualizer::refreshPositions(std::function<sf::Vector2f(size_t)> positionFn) {
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        m_nodes[i].position = positionFn(i);
    }
}

bool Visualizer::saveFramesDAO(const std::string& dirPath) { exportFrames(dirPath); return true; }

void Visualizer::exportAsMP4(const std::string& dirPath, const std::string& mp4File, const ds::VideoConfig& cfg) {
    // Garante PNGs existentes
    exportFrames(dirPath);
    ds::VideoExporter ve;
    ve.exportFromPNGs(dirPath, mp4File, cfg,
        [](const ds::ExportEvent& ev){ if (ev.type == ds::ExportEventType::Error) std::cerr << "[Video] " << ev.message << '\n'; }, nullptr);
}

void Visualizer::exportAsMP4WithProgress(const std::string& dirPath, const std::string& mp4File, const ds::VideoConfig& cfg,
                                         const ds::VideoExporter::EventFn& onEvent,
                                         const ds::VideoExporter::CancelFn& shouldCancel,
                                         int* outPid) {
    exportFrames(dirPath);
    ds::VideoExporter ve;
    ve.exportFromPNGs(dirPath, mp4File, cfg, onEvent, shouldCancel, outPid);
}

void Visualizer::clearMemoryFrames() { m_frameStore.clear(); std::cout << "[Visualizer] Frames em memória limpos." << '\n'; }

bool Visualizer::clearSavedFrames(const std::string& dirPath) {
    namespace fs = std::filesystem;
    if (!fs::exists(dirPath)) return true;
    size_t removed = 0; std::error_code ec;
    for (auto& e : fs::directory_iterator(dirPath, ec)) {
        if (ec) break;
        if (e.is_regular_file()) {
            auto name = e.path().filename().string();
            if (name.rfind("frame_",0)==0 && e.path().extension()==".png") {
                fs::remove(e.path(), ec); if (!ec) ++removed;
            }
        }
    }
    if (!ec) std::cout << "[Visualizer] Removidos " << removed << " PNGs de " << dirPath << '\n';
    return !ec;
}