#include "Visualizer.h"
#include "AnimationStrategy.h"
#include "PNGWriter.hpp"
#include "VideoExporter.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>

namespace {
    sf::Vector2f genericPositionFor(size_t index) {
        float x = 50.f + static_cast<float>(index) * 80.f;
        float y = 300.f;
        return {x, y};
    }
}

void Visualizer::render(const std::vector<int>& state) {
    if (m_strategy) {
        auto steps = m_strategy->animate(state, m_nodes);
        for (auto& st : steps) enqueueAnimation(std::move(st));
        return;
    }
    if (m_nodes.size() < state.size()) m_nodes.resize(state.size());
    for (size_t i = 0; i < state.size(); ++i) {
        m_nodes[i].value = state[i];
        if (m_nodes[i].color == sf::Color()) m_nodes[i].color = sf::Color::Cyan;
        if (m_nodes[i].position == sf::Vector2f()) m_nodes[i].position = genericPositionFor(i);
    }
}

void Visualizer::animateInsert(int value, size_t index) {
    queueOperation("Insert genérico", [this, value, index]() {
        size_t idx = index; if (idx > m_nodes.size()) idx = m_nodes.size();
        sf::Vector2f target = genericPositionFor(idx);
        sf::Vector2f start = { target.x, target.y - 120.f };
        enqueueAnimation(std::make_unique<DataInsertStep>(value, idx, start));
        enqueueAnimation(std::make_unique<MoveStep>(idx, target, 0.35f));
        for (size_t i = 0; i < m_nodes.size(); ++i) {
            enqueueAnimation(std::make_unique<MoveStep>(i, genericPositionFor(i), 0.25f));
            enqueueAnimation(std::make_unique<ColorStep>(i, sf::Color::Cyan, 0.12f));
        }
    });
}

void Visualizer::animateRemove(size_t index) {
    queueOperation("Remove genérico", [this, index]() {
        if (index >= m_nodes.size()) return;
        enqueueAnimation(std::make_unique<ColorStep>(index, sf::Color::Red, 0.18f));
        enqueueAnimation(std::make_unique<MoveStep>(index, sf::Vector2f(m_nodes[index].position.x, m_nodes[index].position.y - 110.f), 0.35f));
        enqueueAnimation(std::make_unique<DataRemoveStep>(index));
        for (size_t i = 0; i < m_nodes.size(); ++i) {
            enqueueAnimation(std::make_unique<MoveStep>(i, genericPositionFor(i), 0.25f));
        }
    });
}

void Visualizer::animateClear() {
    queueOperation("Clear genérico", [this]() {
        for (size_t i = 0; i < m_nodes.size(); ++i) {
            enqueueAnimation(std::make_unique<ColorStep>(i, sf::Color(255, 100, 100), 0.10f));
            enqueueAnimation(std::make_unique<MoveStep>(i, sf::Vector2f(m_nodes[i].position.x, m_nodes[i].position.y - 100.f), 0.25f));
        }
        enqueueAnimation(std::make_unique<ClearAllStep>());
    });
}

void Visualizer::highlight(size_t index) {
    if (index >= m_nodes.size()) return;
    enqueueAnimation(std::make_unique<ColorStep>(index, sf::Color::Yellow, 0.25f));
}

void Visualizer::captureFrame(const sf::RenderWindow& window) {
    if (!m_frameStore.enabled()) return;
    auto size = window.getSize();
    if (size.x == 0 || size.y == 0) return;
    sf::Texture tex; tex.create(size.x, size.y); tex.update(window);
    sf::Image img = tex.copyToImage();
    m_frameStore.capture([&]() {
        ds::FrameData fd; fd.width = size.x; fd.height = size.y;
        const sf::Uint8* px = img.getPixelsPtr();
        fd.pixels.assign(px, px + fd.width * fd.height * 4);
        return fd;
    });
}

void Visualizer::exportFrames(const std::string& dirPath, const std::string& prefix) {
    if (m_frameStore.frames().size() < 5) {
        bool prev = m_frameStore.enabled(); if (!prev) m_frameStore.enable(true);
        const size_t synthCount = 30;
        sf::Vector2u baseSize(1280, 720);
        for (size_t f = 0; f < synthCount; ++f) {
            sf::RenderTexture rt; rt.create(baseSize.x, baseSize.y); rt.clear(sf::Color(28,28,32));
            sf::Font* fontPtr = nullptr;
            float wiggle = std::sin(static_cast<float>(f) * 0.2f) * 6.f;
            for (size_t i=0;i<m_nodes.size();++i) {
                sf::Vector2f pos = (m_nodes[i].position==sf::Vector2f())?genericPositionFor(i):m_nodes[i].position;
                pos.y += wiggle;
                sf::RectangleShape box({70.f,70.f}); box.setPosition(pos); box.setFillColor(sf::Color::Transparent);
                sf::Color edge = (m_nodes[i].color==sf::Color()?sf::Color::Cyan:m_nodes[i].color);
                float pulse = 0.5f + 0.5f * std::sin(static_cast<float>(f)*0.3f + i);
                edge.r = static_cast<sf::Uint8>(std::min(255.f, edge.r * (0.6f + 0.4f * pulse) + 40.f * pulse));
                edge.g = static_cast<sf::Uint8>(std::min(255.f, edge.g * (0.6f + 0.4f * pulse)));
                edge.b = static_cast<sf::Uint8>(std::min(255.f, edge.b * (0.6f + 0.4f * pulse)));
                box.setOutlineColor(edge); box.setOutlineThickness(2.f);
                rt.draw(box);
            }
            float pct = static_cast<float>(f+1)/synthCount;
            sf::RectangleShape barBg({400.f,16.f}); barBg.setPosition(40.f, baseSize.y - 50.f); barBg.setFillColor(sf::Color(50,50,60)); rt.draw(barBg);
            sf::RectangleShape bar({400.f*pct,16.f}); bar.setPosition(40.f, baseSize.y - 50.f); bar.setFillColor(sf::Color(90,170,255)); rt.draw(bar);
            rt.display(); sf::Image img = rt.getTexture().copyToImage();
            m_frameStore.capture([&]{ ds::FrameData fd; fd.width=img.getSize().x; fd.height=img.getSize().y; const sf::Uint8* px=img.getPixelsPtr(); fd.pixels.assign(px, px+fd.width*fd.height*4); return fd; });
        }
        if (!prev) m_frameStore.enable(false);
    }
    ds::PNGWriter writer; writer.save(m_frameStore.frames(), dirPath, prefix, nullptr, nullptr);
}

void Visualizer::exportFramesWithProgress(const std::string& dirPath, const std::string& prefix,
                                  const ds::PNGWriter::EventFn& onEvent,
                                  const ds::PNGWriter::CancelFn& shouldCancel) {
    if (m_frameStore.frames().size() < 5) {
        bool prev = m_frameStore.enabled(); if (!prev) m_frameStore.enable(true);
        const size_t synthCount = 30;
        sf::Vector2u baseSize(1280, 720);
        if (onEvent) onEvent({ds::ExportEventType::Start,0,synthCount,0.0,"Gerando frames sintéticos"});
        for (size_t f=0; f<synthCount; ++f) {
            if (shouldCancel && shouldCancel()) { if (onEvent) onEvent({ds::ExportEventType::Cancelled,f,synthCount,(double)f*100.0/synthCount,"Cancelado"}); break; }
            sf::RenderTexture rt; rt.create(baseSize.x, baseSize.y); rt.clear(sf::Color(22,22,26));
            float wiggle = std::sin(static_cast<float>(f) * 0.22f) * 5.f;
            for (size_t i=0;i<m_nodes.size();++i) {
                sf::Vector2f pos = (m_nodes[i].position==sf::Vector2f())?genericPositionFor(i):m_nodes[i].position; pos.y += wiggle;
                sf::CircleShape circ(30.f); circ.setPosition(pos); sf::Color base = (m_nodes[i].color==sf::Color()?sf::Color::Cyan:m_nodes[i].color);
                float pulse = 0.6f + 0.4f * std::sin(static_cast<float>(f)*0.25f + i);
                base.a = 255; base.r = (sf::Uint8)std::min(255.f, base.r * pulse + 40.f);
                circ.setFillColor(base); circ.setOutlineThickness(2.f); circ.setOutlineColor(sf::Color::White); rt.draw(circ);
            }
            rt.display(); sf::Image img = rt.getTexture().copyToImage();
            m_frameStore.capture([&]{ ds::FrameData fd; fd.width=img.getSize().x; fd.height=img.getSize().y; const sf::Uint8* px=img.getPixelsPtr(); fd.pixels.assign(px, px+fd.width*fd.height*4); return fd; });
            if (onEvent) onEvent({ds::ExportEventType::FrameSaved,f+1,synthCount,(double)(f+1)*100.0/synthCount,"Gerado"});
        }
        if (!prev) m_frameStore.enable(false);
    size_t produced = m_frameStore.frames().size();
    if (onEvent) onEvent({ds::ExportEventType::Completed,produced,produced,100.0,"Frames sintéticos"});
    }
    ds::PNGWriter writer; writer.save(m_frameStore.frames(), dirPath, prefix, onEvent, shouldCancel);
}

void Visualizer::exportAsMP4(const std::string& dirPath, const std::string& mp4File, const ds::VideoConfig& cfg) {
    exportFrames(dirPath, "frame");
    auto exporter = ds::VideoExporter::createDefault();
    exporter->exportFromPNGs(dirPath, mp4File, cfg, nullptr, nullptr, nullptr);
}

void Visualizer::exportAsMP4WithProgress(const std::string& dirPath, const std::string& mp4File, const ds::VideoConfig& cfg,
                                 const ds::VideoExporter::EventFn& onEvent,
                                 const ds::VideoExporter::CancelFn& shouldCancel,
                                 int* outPid) {
    exportFramesWithProgress(dirPath, "frame", nullptr, shouldCancel);
    auto exporter = ds::VideoExporter::createDefault();
    exporter->exportFromPNGs(dirPath, mp4File, cfg, onEvent, shouldCancel, outPid);
}

void Visualizer::refreshPositions(std::function<sf::Vector2f(size_t)> positionFn) {
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        m_nodes[i].position = positionFn(i);
    }
}

bool Visualizer::saveFramesDAO(const std::string& dirPath) {
    ds::PNGWriter writer; return writer.save(m_frameStore.frames(), dirPath, "frame", nullptr, nullptr);
}

void Visualizer::clearMemoryFrames() {
    m_frameStore.clear();
}

bool Visualizer::clearSavedFrames(const std::string& dirPath) {
    namespace fs = std::filesystem;
    std::error_code ec;
    if (!fs::exists(dirPath)) return true;
    size_t removed = 0;
    for (auto& e : fs::directory_iterator(dirPath, ec)) {
        if (ec) break;
        if (e.is_regular_file() && e.path().extension() == ".png") {
            fs::remove(e.path(), ec); if (!ec) ++removed;
        }
    }
    if (ec) return false;
    std::cout << "[Visualizer] Removidos " << removed << " PNGs de '" << dirPath << "'\n";
    return true;
}