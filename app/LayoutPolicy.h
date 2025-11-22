#pragma once
#include <SFML/Graphics.hpp>
#include <cstddef>

class ILayoutPolicy {
public:
    virtual ~ILayoutPolicy() = default;
    virtual sf::Vector2f positionForIndex(std::size_t index, sf::Vector2f origin) const = 0;
};

class LinearLayoutPolicy : public ILayoutPolicy {
    float m_elementWidth;
    float m_spacing;
public:
    LinearLayoutPolicy(float elementWidth, float spacing)
        : m_elementWidth(elementWidth), m_spacing(spacing) {}
    sf::Vector2f positionForIndex(std::size_t index, sf::Vector2f origin) const override {
        float x = origin.x + static_cast<float>(index) * (m_elementWidth + m_spacing);
        return { x, origin.y };
    }
};

class ListLayoutPolicy : public ILayoutPolicy {
    float m_nodeWidth;
    float m_ptrWidth;
    float m_spacing;
public:
    ListLayoutPolicy(float nodeWidth, float ptrWidth, float spacing)
        : m_nodeWidth(nodeWidth), m_ptrWidth(ptrWidth), m_spacing(spacing) {}
    sf::Vector2f positionForIndex(std::size_t index, sf::Vector2f origin) const override {
        float stride = m_nodeWidth + m_ptrWidth + m_spacing;
        float x = origin.x + static_cast<float>(index) * stride;
        return { x, origin.y };
    }
};