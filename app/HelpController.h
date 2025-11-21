#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "commands_help.hpp"
#include <string>

class HelpController {
public:
    explicit HelpController(sf::Font& font) : m_font(font) {}

    void toggle() {
        if (!m_show) {
            m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 700), "Comandos Disponiveis", sf::Style::Titlebar | sf::Style::Close);
            m_window->setFramerateLimit(60);
            m_show = true;
        } else {
            if (m_window && m_window->isOpen()) m_window->close();
            m_show = false;
        }
    }

    bool isOpen() const { return m_show && m_window && m_window->isOpen(); }

    void processEvents() {
        if (!isOpen()) return;
        sf::Event e;
        while (m_window->pollEvent(e)) {
            if (e.type == sf::Event::Closed) { m_window->close(); m_show = false; }
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) { m_window->close(); m_show = false; }
        }
    }

    void render() {
        if (!isOpen()) return;
    m_window->clear(sf::Color(25,25,25));
    sf::Text header("Lista de Comandos", m_font, appstyle::HELP_HEADER); header.setFillColor(sf::Color(240,240,240)); header.setPosition(20.f, 15.f); m_window->draw(header);
    float y = 60.f; sf::Text line("", m_font, appstyle::HELP_LINE);
        for (const auto &p : getCommandHelp()) {
            line.setString(std::string("[") + p.key + "] " + p.description);
            line.setFillColor(sf::Color(200,200,200)); line.setPosition(30.f, y); m_window->draw(line); y += 26.f;
        }
    sf::Text footer("ESC para fechar", m_font, appstyle::HELP_LINE); footer.setFillColor(sf::Color(180,180,180)); footer.setPosition(30.f, m_window->getSize().y - 45.f); m_window->draw(footer);
        m_window->display();
    }
private:
    sf::Font& m_font;
    std::unique_ptr<sf::RenderWindow> m_window;
    bool m_show = false;
};
