#pragma once
#include "IDataView.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class VectorView : public IDataView {
public:
    VectorView(sf::Font& font, const sf::Vector2f& position)
        : m_font(font), m_position(position) {
    }
    
    virtual ~VectorView() = default;

    void render(sf::RenderWindow& window) const override {
        for (size_t i = 0; i < m_renderNodes.size(); ++i) {
            const auto& node = m_renderNodes[i];

            sf::RectangleShape box(sf::Vector2f(BOX_WIDTH, BOX_HEIGHT));
            box.setPosition(node.position);
            box.setFillColor(node.color);
            box.setOutlineColor(sf::Color::Black);
            box.setOutlineThickness(2.0f);
            window.draw(box);

            sf::Text text(std::to_string(node.value), m_font, 24);
            text.setFillColor(sf::Color::Black);
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setPosition(
                node.position.x + (BOX_WIDTH - textBounds.width) / 2,
                node.position.y + (BOX_HEIGHT - textBounds.height) / 2
            );
            window.draw(text);

            sf::Text indexText(std::to_string(i), m_font, 16);
            indexText.setFillColor(sf::Color::White);
            indexText.setPosition(node.position.x + 2, node.position.y - 20);
            window.draw(indexText);
        }
    }
    
    void updateData(const std::vector<int>& data) override {
        m_renderNodes.clear();
        for (size_t i = 0; i < data.size(); ++i) {
            RenderNode node;
            node.value = data[i];
            node.position = getPositionForIndex(i);
            node.color = sf::Color::Cyan;
            m_renderNodes.push_back(node);
        }
    }
    
    void setPosition(const sf::Vector2f& position) override {
        m_position = position;
        for (size_t i = 0; i < m_renderNodes.size(); ++i) {
            m_renderNodes[i].position = getPositionForIndex(i);
        }
    }
    
    void reflow(float windowWidth, float panelWidth) override {
        float availableWidth = windowWidth - panelWidth - 40;
        if (!m_renderNodes.empty()) {
            float totalWidth = m_renderNodes.size() * (BOX_WIDTH + SPACING) - SPACING;
            if (totalWidth > availableWidth) {
                m_position.x = 20;
            } else {
                m_position.x = (availableWidth - totalWidth) / 2 + 20;
            }
            for (size_t i = 0; i < m_renderNodes.size(); ++i) {
                m_renderNodes[i].position = getPositionForIndex(i);
            }
        }
    }
    
    void highlight(size_t index) override {
        if (index < m_renderNodes.size()) {
            m_renderNodes[index].color = sf::Color::Yellow;
        }
    }
    
    bool hasActiveAnimations() const override {
        return false;
    }
    
    void updateAnimations(float deltaTime) override {
        (void)deltaTime;
    }

private:
    struct RenderNode {
        int value;
        sf::Vector2f position;
        sf::Color color = sf::Color::Cyan;
    };
    
    sf::Vector2f getPositionForIndex(size_t i) const {
        return sf::Vector2f(m_position.x + i * (BOX_WIDTH + SPACING), m_position.y);
    }
    sf::Font& m_font;
    sf::Vector2f m_position;
    std::vector<RenderNode> m_renderNodes;
    static constexpr float BOX_WIDTH = 60.0f;
    static constexpr float BOX_HEIGHT = 40.0f;
    static constexpr float SPACING = 10.0f;
};