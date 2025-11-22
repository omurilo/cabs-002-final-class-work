#pragma once
#include <SFML/Graphics.hpp>
#include "HUDModel.h"
#include "AppStyle.h"
#include <vector>

class HUDView {
public:
    explicit HUDView(sf::Font& font) : m_font(font) {}
    
    void syncState(const HUDState& state) {
        m_state = state;
    }
    
    void draw(sf::RenderWindow& window) {
        buildHUDTexts();
        drawPanel(window);
        drawTexts(window);
        drawProgressBar(window);
    }
    
    sf::FloatRect getBounds() const {
        if (m_hudTexts.empty()) return {};
        return m_panelBounds;
    }
    
private:
    void buildHUDTexts() {
        m_hudTexts.clear();
        m_hudY = 18.f;
        m_hudMaxWidth = 0.f;
        
        auto createHudLine = [&](const std::string& text, unsigned size, sf::Color color) {
            sf::Text t(text, m_font, size);
            sf::FloatRect bounds = t.getLocalBounds();
            m_hudMaxWidth = std::max(m_hudMaxWidth, bounds.width);
            t.setFillColor(color);
            t.setPosition(m_hudStartX + 8.f, m_hudY);
            m_hudTexts.push_back(t);
            m_hudY += size * 1.15f;
        };
        
        createHudLine(
            m_state.recording ? "REC ON (G)" : "REC OFF (G)",
            appstyle::HUD_MEDIUM,
            m_state.recording ? sf::Color(255, 80, 80) : sf::Color(200, 200, 200)
        );
        
        createHudLine(
            m_state.capturing ? "CAPTURA ON (F)" : "CAPTURA OFF (F)",
            appstyle::HUD_MEDIUM,
            m_state.capturing ? sf::Color(80, 220, 80) : sf::Color(180, 180, 180)
        );
        
        if (m_state.showLimitStatus) {
            createHudLine(
                "Limite=" + std::to_string(m_state.captureLimit) + " (T)",
                appstyle::HUD_SMALL,
                sf::Color(180, 255, 180)
            );
        }
        
        if (m_state.replayActive) {
            std::string replayText = "Replay: " + std::to_string(static_cast<int>(m_state.replayClock * 1000)) + "ms";
            if (m_state.replayPaused) {
                replayText += " [PAUSED]";
            }
            replayText += " x" + std::to_string(m_state.replaySpeed);
            createHudLine(replayText, appstyle::HUD_SMALL, sf::Color(180, 200, 255));
        }

        if (m_state.capturing) {
            createHudLine("Frames: (E/M p/ export)", appstyle::HUD_SMALL, sf::Color(150, 220, 160));
        }
        
        if (m_state.exportingFrames) {
            createHudLine(
                "Export PNG: " + std::to_string(m_state.framesCurrent) + "/" + std::to_string(m_state.framesTotal),
                appstyle::HUD_SMALL,
                sf::Color(255, 180, 80)
            );
        }
        
        if (m_state.exportingVideo) {
            createHudLine(
                "Export MP4: " + m_state.videoProgressLine,
                appstyle::HUD_SMALL,
                sf::Color(255, 200, 120)
            );
        }
        
        if (m_state.showCompletionMessage) {
            createHudLine(
                m_state.completionMessage,
                appstyle::HUD_SMALL,
                sf::Color(80, 255, 80)  
            );
        }
    }
    
    void drawPanel(sf::RenderWindow& window) {
        float panelHeight = m_hudY - 14.f + 10.f;
        sf::RectangleShape hudBg(sf::Vector2f(m_hudMaxWidth + 40.f, panelHeight));
        hudBg.setPosition(m_hudStartX, 8.f);
        hudBg.setFillColor(sf::Color(0, 0, 0, 120));
        hudBg.setOutlineThickness(1.f);
        hudBg.setOutlineColor(sf::Color(70, 70, 70, 180));
        
        m_panelBounds = hudBg.getGlobalBounds();
        window.draw(hudBg);
    }
    
    void drawTexts(sf::RenderWindow& window) {
        for (const auto& text : m_hudTexts) {
            window.draw(text);
        }
    }
    
    void drawProgressBar(sf::RenderWindow& window) {
        if (!m_state.exportingFrames || m_state.framesTotal == 0) return;
        
        float barW = m_hudMaxWidth + 8.f;
        float barH = 10.f;
        float barX = m_hudStartX + 8.f;
        float barY = m_panelBounds.top + m_panelBounds.height - 18.f;
        
        sf::RectangleShape bg(sf::Vector2f(barW, barH));
        bg.setPosition(barX, barY);
        bg.setFillColor(sf::Color(50, 50, 50));
        window.draw(bg);
        
        float pct = std::min(1.f, std::max(0.f, static_cast<float>(m_state.framesCurrent) / static_cast<float>(m_state.framesTotal)));
        sf::RectangleShape fg(sf::Vector2f(barW * pct, barH));
        fg.setPosition(barX, barY);
        fg.setFillColor(sf::Color(100, 200, 100));
        window.draw(fg);
    }
    
    sf::Font& m_font;
    HUDState m_state;
    std::vector<sf::Text> m_hudTexts;
    float m_hudY = 18.f;
    float m_hudStartX = 12.f;
    float m_hudMaxWidth = 0.f;
    sf::FloatRect m_panelBounds;
};