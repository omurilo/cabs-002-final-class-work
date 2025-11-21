#pragma once
#include "IControlPanelView.h"
#include <SFML/Graphics.hpp>
#include "VectorVisualizer.h"
#include "LinkedListVisualizer.h"

class ControlPanelView : public IControlPanelView {
public:
    ControlPanelView(sf::RenderWindow& window, sf::Font& font, 
                    const VectorVisualizer& vecViz, const LinkedListVisualizer& listViz)
        : window(window), font(font), vecViz(vecViz), listViz(listViz) {}
    
    void showCommands(const std::vector<std::string>& commands) override;
    void showStatus(const StatusInfo& status) override;
    void showProgress(float percentage) override;
    
private:
    sf::RenderWindow& window; 
    sf::Font& font; 
    const VectorVisualizer& vecViz; 
    const LinkedListVisualizer& listViz; 
};