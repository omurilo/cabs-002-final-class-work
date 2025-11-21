#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <filesystem>
#include <unordered_map>
#include <string>
#include "VectorController.h"
#include "LinkedListController.h"
#include "VectorVisualizer.h"
#include "LinkedListVisualizer.h"
#include "ReplayController.h"
#include "ExportController.h"
#include "datastructures.hpp"

class InputController {
public:
    using Action = std::function<void()>;

    InputController(
        VectorController& vCtrl,
        LinkedListController& lCtrl,
        VectorVisualizer& vViz,
        ds::CommandRecorder& recorder,
        ReplayController& replay,
        ExportController& exportCtrl,
        ds::RandomProvider& rng,
        std::function<void(const std::string&)> pushSubtitle,
        bool& showLimitFlag,
        std::function<void()> helpToggle
    ) : m_vectorCtrl(vCtrl), m_listCtrl(lCtrl), m_vectorViz(vViz),
        m_recorder(recorder), m_replay(replay), m_export(exportCtrl), m_rng(rng),
        m_push(std::move(pushSubtitle)), m_showLimit(showLimitFlag), m_helpToggle(std::move(helpToggle)) {
        registerDefaults();
    }

    void handleKey(sf::Keyboard::Key key) {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            it->second();
        }
    }
private:
    void registerDefaults() {
        m_map[sf::Keyboard::I] = [&]{ m_vectorCtrl.insert(); };
        m_map[sf::Keyboard::R] = [&]{ m_vectorCtrl.remove(); };
        m_map[sf::Keyboard::A] = [&]{ m_listCtrl.insert(); };
        m_map[sf::Keyboard::D] = [&]{ m_listCtrl.remove(); };
        m_map[sf::Keyboard::H] = [&]{ m_vectorCtrl.highlight(); };
        m_map[sf::Keyboard::V] = [&]{ m_vectorCtrl.clear(); };
        m_map[sf::Keyboard::B] = [&]{ m_listCtrl.clear(); };
        m_map[sf::Keyboard::E] = [&]{ m_export.startPNG(&m_vectorViz, "frames/vector", "frame"); };
        m_map[sf::Keyboard::M] = [&]{ ds::VideoConfig cfg; cfg.fps=30; cfg.crf=28; m_export.startMP4(&m_vectorViz, "frames/vector", "vector.mp4", cfg); };
        m_map[sf::Keyboard::Z] = [&]{ m_export.requestCancelVideo(); m_export.requestCancelFrames(); };
        m_map[sf::Keyboard::F] = [&]{ m_vectorViz.toggleCapture(); m_push(m_vectorViz.isCaptureEnabled()?"Captura ON":"Captura OFF"); };
        m_map[sf::Keyboard::C] = [&]{ m_vectorViz.clearMemoryFrames(); m_push("Frames memoria limpos"); };
        m_map[sf::Keyboard::X] = [&]{ m_vectorViz.clearSavedFrames("frames/vector"); m_push("Frames disco limpos"); };
        m_map[sf::Keyboard::T] = [&]{ size_t currentLimit = m_vectorViz.getCaptureLimit(); size_t newLimit = (currentLimit <= 900)?1800:900; m_vectorViz.setCaptureMaxFrames(newLimit); m_showLimit=true; m_push("Limite="+std::to_string(newLimit)); };
        m_map[sf::Keyboard::G] = [&]{ bool wasRecording = m_recorder.isRecording(); m_recorder.toggle(); if(!wasRecording && m_recorder.isRecording()){ if(!m_rng.hasSeed()) m_rng.setSeed(m_recorder.seed()); m_rng.setSeed(m_recorder.seed()); m_push("Seed aplicada="+std::to_string(m_recorder.seed())); } m_replay.startTemporal(); m_push("Replay temporal ON"); };
        m_map[sf::Keyboard::S] = [&]{
            const std::string out = "commands.json";
            if (m_recorder.save(out)) {
                m_push("Salvo " + std::to_string(m_recorder.get().size()) + " cmds em commands.json");
            } else {
                m_push("Falha salvar commands.json");
            }
        };
        m_map[sf::Keyboard::K] = [&]{
            const std::string path = "commands.json";
            size_t before = m_recorder.get().size();
            if (std::filesystem::exists(path)) {
                if (m_recorder.load(path)) {
                    size_t after = m_recorder.get().size();
                    if(!m_rng.hasSeed()) m_rng.setSeed(m_recorder.seed());
                    if (after != before) {
                        m_push("Replay carregado: " + std::to_string(after) + " cmds");
                    } else {
                        m_push("Replay recarregado");
                    }
                } else {
                    m_push("Falha carregar commands.json");
                }
            } else if (before==0) {
                m_push("commands.json ausente");
            }
            if (m_replay.startTemporal()) {
                m_push("Replay temporal ON (K)");
            } else if (!m_replay.active()) {
                m_push("Replay NAO iniciado");
            }
        };
        m_map[sf::Keyboard::P] = [&]{ if(m_replay.active()){ m_replay.pauseToggle(); m_push(m_replay.paused()?"Replay PAUSE":"Replay RESUME"); } };
        m_map[sf::Keyboard::N] = [&]{ m_replay.step(); m_push("Replay STEP"); };
        m_map[sf::Keyboard::LBracket] = [&]{ m_replay.speedHalf(); m_push("Replay speed="+std::to_string(m_replay.speed())); };
        m_map[sf::Keyboard::RBracket] = [&]{ m_replay.speedDouble(); m_push("Replay speed="+std::to_string(m_replay.speed())); };
        m_map[sf::Keyboard::Slash] = [&]{ if(m_helpToggle) m_helpToggle(); };
    }

    std::unordered_map<sf::Keyboard::Key, Action> m_map;
    VectorController& m_vectorCtrl;
    LinkedListController& m_listCtrl;
    VectorVisualizer& m_vectorViz;
    ds::CommandRecorder& m_recorder;
    ReplayController& m_replay;
    ExportController& m_export;
    ds::RandomProvider& m_rng;
    std::function<void(const std::string&)> m_push;
    bool& m_showLimit;
    std::function<void()> m_helpToggle;
};
