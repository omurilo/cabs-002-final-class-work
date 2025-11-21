#include "HelpView.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <string>

static const std::vector<std::pair<std::string, std::string>> COMMAND_HELP = {
    {"I", "Inserir elemento aleatorio no Vetor"},
    {"R", "Remover elemento (indice aleatorio) do Vetor"},
    {"H", "Destacar elemento (indice aleatorio) no Vetor"},
    {"V", "Limpar todos os elementos do Vetor"},
    {"A", "Inserir elemento aleatorio na Lista"},
    {"D", "Remover elemento (posicao aleatoria) da Lista"},
    {"B", "Limpar todos os elementos da Lista"},
    {"F", "Toggle capturar frames em memoria"},
    {"C", "Limpar frames em memoria"},
    {"X", "Limpar frames salvos em disco"},
    {"E", "Exportar frames em PNG (frames/vector)"},
    {"M", "Exportar MP4 (vector.mp4) via ffmpeg"},
    {"G", "Iniciar/Parar gravacao de comandos"},
    {"J", "Salvar comandos em JSON (commands.json)"},
    {"K", "Carregar JSON e iniciar replay temporal"},
    {"P", "Pausar/Retomar replay temporal"},
    {"N", "Avancar um passo no replay quando pausado"},
    {"[", "Diminuir velocidade do replay temporal"},
    {"]", "Aumentar velocidade do replay temporal"},
    {"B", "(Futuro) Toggle modo circular de frames"},
    {"T", "Toggle limite de frames (900 <-> 1800)"}
};

void HelpPanel::showHelpWindow(sf::Font& font) {
    static bool showHelpWindow = false;
    static std::unique_ptr<sf::RenderWindow> helpWindow;
    
    if (!showHelpWindow) {
        showHelpWindow = true;
        helpWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 700), "Comandos Disponiveis", sf::Style::Titlebar | sf::Style::Close);
    }
    
    if (helpWindow && helpWindow->isOpen()) {
        sf::Event hevent;
        while (helpWindow->pollEvent(hevent)) {
            if (hevent.type == sf::Event::Closed) {
                helpWindow->close();
                showHelpWindow = false;
            }
            if (hevent.type == sf::Event::KeyPressed && hevent.key.code == sf::Keyboard::Escape) {
                helpWindow->close(); 
                showHelpWindow = false;
            }
        }
        
        helpWindow->clear(sf::Color(25, 25, 25));
        sf::Text header("Lista de Comandos", font, 24);
        header.setFillColor(sf::Color(240,240,240));
        header.setPosition(20.f, 15.f);
        helpWindow->draw(header);

        float y = 60.f;
        sf::Text line("", font, 18);
        for (const auto &p : COMMAND_HELP) {
            line.setString("[" + p.first + "] " + p.second);
            line.setFillColor(sf::Color(200,200,200));
            line.setPosition(30.f, y);
            helpWindow->draw(line);
            y += 26.f;
        }

        sf::Text footer("ESC para fechar", font, 14);
        footer.setFillColor(sf::Color(150,150,150));
        footer.setPosition(30.f, y + 20.f);
        helpWindow->draw(footer);
        helpWindow->display();
    }
}