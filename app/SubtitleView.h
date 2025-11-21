#pragma once
#include <SFML/Graphics.hpp>
#include "SubtitleModel.h"
#include "AppStyle.h"

class SubtitleView {
public:
    explicit SubtitleView(sf::Font& font) : m_font(font) {}
    
    void syncState(const std::vector<Subtitle>& subtitles) {
        m_subtitles = subtitles;
    }
    
    void draw(sf::RenderWindow& window) const {
        if (m_subtitles.empty()) return;
        
        float baseY = window.getSize().y - 30.f;
        sf::Text subtitleText("", m_font, appstyle::HUD_MEDIUM);
        
        for (int i = static_cast<int>(m_subtitles.size()) - 1, line = 0; i >= 0; --i, ++line) {
            const auto& subtitle = m_subtitles[i];
            float alphaRatio = std::max(0.f, 3.5f - subtitle.age) / 3.5f;
            sf::Color color(230, 230, 230, static_cast<sf::Uint8>(alphaRatio * 255));
            
            subtitleText.setFillColor(color);
            subtitleText.setString(subtitle.text);
            
            sf::FloatRect bounds = subtitleText.getLocalBounds();
            subtitleText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            subtitleText.setPosition(window.getSize().x / 2.f, baseY - line * 22.f);
            
            window.draw(subtitleText);
        }
    }
    
private:
    sf::Font& m_font;
    std::vector<Subtitle> m_subtitles;
};