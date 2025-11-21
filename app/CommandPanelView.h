#pragma once
#include <SFML/Graphics.hpp>
#include "IView.h"
#include "VisualizerBase.h"
#include <vector>
#include <string>

class CommandPanelView {
public:
    explicit CommandPanelView(float width = 280.f) : m_width(width) {}

    void addView(const IVisualizer* viz) { m_visualizers.push_back(viz); }
    float width() const { return m_width; }

    void draw(sf::RenderWindow& window, sf::Font& font) const {
        float panelX = window.getSize().x - m_width;
        sf::RectangleShape panel(sf::Vector2f(m_width, window.getSize().y));
        panel.setPosition(panelX, 0);
        panel.setFillColor(sf::Color(20,20,20,200));
        window.draw(panel);

        sf::Text title("Fila de Comandos Pendentes", font, 20);
        title.setFillColor(sf::Color::White);
        title.setPosition(panelX + 15, 15);
        window.draw(title);

        std::vector<std::string> commands;
        for (auto* v : m_visualizers) {
            if (!v) continue;
            const auto& q = v->getOperationQueue();
            for (const auto& c : q) commands.push_back(c.description);
        }
        sf::Text line("", font, 16);
        line.setFillColor(sf::Color(220,220,220));
        float y = 60.f;
        if (commands.empty()) {
            line.setString("(vazio)");
            line.setPosition(panelX + 15, y);
            window.draw(line);
        } else {
            for (const auto& c : commands) {
                line.setString(c);
                line.setPosition(panelX + 15, y);
                window.draw(line);
                y += 25.f;
            }
        }
        sf::Text stats("Pendentes: " + std::to_string(commands.size()), font, 14);
        stats.setFillColor(sf::Color(180,180,180));
        stats.setPosition(panelX + 15, window.getSize().y - 30.f);
        window.draw(stats);
    }
private:
    float m_width;
    std::vector<const IVisualizer*> m_visualizers;
};
