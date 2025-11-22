#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include "VisualNode.h"

class AnimationStep {
public:
    virtual ~AnimationStep() = default;
    virtual bool update(std::vector<VisualNode>& nodes, float dt) = 0;
};

class ColorStep : public AnimationStep {
    size_t m_index; 
    sf::Color m_target; 
    sf::Color m_start; 
    float m_duration; 
    float m_progress = 0.f;
public:
    ColorStep(size_t index, sf::Color target, float duration = 0.3f)
        : m_index(index), m_target(target), m_duration(duration) {}
    bool update(std::vector<VisualNode>& nodes, float dt) override {
        if (m_index >= nodes.size()) return true;
        if (m_progress == 0.f) m_start = nodes[m_index].color;
        m_progress += dt / m_duration;
        float t = (m_progress > 1.f ? 1.f : m_progress);
        auto lerp = [&](sf::Uint8 a, sf::Uint8 b) { return static_cast<sf::Uint8>(a + (b - a) * t); };
        nodes[m_index].color = sf::Color(
            lerp(m_start.r, m_target.r),
            lerp(m_start.g, m_target.g),
            lerp(m_start.b, m_target.b)
        );
        return m_progress >= 1.f;
    }
};

class MoveStep : public AnimationStep {
    size_t m_index; 
    sf::Vector2f m_target; 
    sf::Vector2f m_start; 
    float m_duration; 
    float m_progress = 0.f;
public:
    MoveStep(size_t index, sf::Vector2f target, float duration = 0.5f)
        : m_index(index), m_target(target), m_duration(duration) {}
    bool update(std::vector<VisualNode>& nodes, float dt) override {
        if (m_index >= nodes.size()) return true;
        if (m_progress == 0.f) m_start = nodes[m_index].position;
        m_progress += dt / m_duration;
        float t = (m_progress > 1.f ? 1.f : m_progress);
        nodes[m_index].position = m_start + (m_target - m_start) * t;
        return m_progress >= 1.f;
    }
};

class DataInsertStep : public AnimationStep {
    int m_value; 
    size_t m_index; 
    sf::Vector2f m_startPos; 
    std::string m_label; 
    bool m_done = false;
public:
    DataInsertStep(int value, size_t index, sf::Vector2f startPos, std::string label = "")
        : m_value(value), m_index(index), m_startPos(startPos), m_label(std::move(label)) {}
    bool update(std::vector<VisualNode>& nodes, float /*dt*/) override {
        if (m_done) return true;
        if (m_index > nodes.size()) m_index = nodes.size();
        VisualNode n; 
        n.value = m_value; 
        n.label = m_label; 
        n.position = m_startPos; 
        n.color = sf::Color::Cyan;
        nodes.insert(nodes.begin() + m_index, n);
        m_done = true; 
        return true;
    }
};

class DataRemoveStep : public AnimationStep {
    size_t m_index; 
    bool m_done = false;
public:
    explicit DataRemoveStep(size_t index) : m_index(index) {}
    bool update(std::vector<VisualNode>& nodes, float /*dt*/) override {
        if (m_done) return true;
        if (m_index < nodes.size()) nodes.erase(nodes.begin() + m_index);
        m_done = true; 
        return true;
    }
};

class ClearAllStep : public AnimationStep {
    bool m_done = false;
public:
    bool update(std::vector<VisualNode>& nodes, float /*dt*/) override {
        if (!m_done) { nodes.clear(); m_done = true; }
        return true;
    }
};