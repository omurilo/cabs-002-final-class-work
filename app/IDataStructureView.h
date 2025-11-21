#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>


class IDataView {
public:
    virtual ~IDataView() = default;
    
    
    virtual void render(sf::RenderWindow& window) const = 0;
    virtual void updateData(const std::vector<int>& data) = 0;
    virtual void setPosition(const sf::Vector2f& position) = 0;
    virtual void reflow(float windowWidth, float panelWidth = 280.0f) = 0;
    
    
    virtual void highlight(size_t index) = 0;
    virtual bool hasActiveAnimations() const = 0;
    virtual void updateAnimations(float deltaTime) = 0;
};


class IViewFactory {
public:
    virtual ~IViewFactory() = default;
    virtual std::unique_ptr<IDataView> createVectorView(sf::Font& font, const sf::Vector2f& position) = 0;
    virtual std::unique_ptr<IDataView> createLinkedListView(sf::Font& font, const sf::Vector2f& position) = 0;
};