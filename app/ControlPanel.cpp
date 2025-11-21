#include "ControlPanelView.h"

void ControlPanelView::showCommands(const std::vector<std::string>& commands) {
    const float panelWidth = 280.f;
    // Posição X calculada a cada frame para se ajustar ao tamanho da janela
    const float panelX = window.getSize().x - panelWidth;

    // Fundo do painel (altura acompanha a da janela)
    sf::RectangleShape panel(sf::Vector2f(panelWidth, window.getSize().y));
    panel.setPosition(panelX, 0);
    panel.setFillColor(sf::Color(20, 20, 20, 200));
    window.draw(panel);

    // Título do painel
    sf::Text title("Fila de Comandos Pendentes", font, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition(panelX + 15, 15);
    window.draw(title);

    const auto& vecQueue = vecViz.getOperationQueue();
    const auto& listQueue = listViz.getOperationQueue();

    std::vector<std::string> allCommands;
    for(const auto& cmd : vecQueue) allCommands.push_back(cmd.description);
    for(const auto& cmd : listQueue) allCommands.push_back(cmd.description);
    
    sf::Text commandText("", font, 16);
    commandText.setFillColor(sf::Color(220, 220, 220));
    float currentY = 60.f;

    if (allCommands.empty()) {
        commandText.setString("(vazio)");
        commandText.setPosition(panelX + 15, currentY);
        window.draw(commandText);
    } else {
        for(const auto& desc : allCommands) {
            commandText.setString(desc);
            commandText.setPosition(panelX + 15, currentY);
            window.draw(commandText);
            currentY += 25.f;
        }
    }
}

void ControlPanelView::showProgress(float percentage) {
    const float panelWidth = 280.f;
    const float panelX = window.getSize().x - panelWidth;
    
    sf::Text progress("Progresso: " + std::to_string(percentage) + "%", font, 14);
    progress.setFillColor(sf::Color(180,180,180));
    progress.setPosition(panelX + 15, window.getSize().y - 60.f);
    window.draw(progress);
}

void ControlPanelView::showStatus(const StatusInfo& status) {
    const float panelWidth = 280.f;
    const float panelX = window.getSize().x - panelWidth;
    
    sf::Text stats("Array: " + status.arrayStatus + " | List: " + status.linkedListStatus, font, 14);
    stats.setFillColor(sf::Color(180,180,180));
    stats.setPosition(panelX + 15, window.getSize().y - 30.f);
    window.draw(stats);
}