#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct VisualNode {
    int value = 0;
    std::string label;
    sf::Vector2f position {0.f, 0.f};
    sf::Color color {sf::Color::Cyan};
};