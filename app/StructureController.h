#pragma once
#include <memory>
#include <string>
#include "Visualizer.h"
#include "../lib/include/datastructures.hpp"

class StructureController {
public:
    StructureController(std::unique_ptr<ds::AbstractDataStructure> s, Visualizer* v, ds::RandomProvider* rng = nullptr)
        : m_structure(std::move(s)), m_visualizer(v), m_rng(rng) {}
    void execute(const std::string& op);
    void executeAndRecord(const std::string& op, ds::CommandRecorder* recorder, const std::string& targetName);
    void insertAt(size_t idx, int val);
    void removeAt(size_t idx);
    void highlightAt(size_t idx);
    void connect() { if (m_structure && m_visualizer) m_visualizer->render(m_structure->getState()); }
    void runAnimation() { }
    void exportFrames(const std::string& path) { if (m_visualizer) m_visualizer->exportFrames(path); }
private:
    std::unique_ptr<ds::AbstractDataStructure> m_structure;
    Visualizer* m_visualizer;
    ds::RandomProvider* m_rng = nullptr;
};