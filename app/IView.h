// Interface base para View (renderização e animações)
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class IView {
public:
    virtual ~IView() = default;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual void syncState(const std::vector<int>& logical) = 0;
    virtual void syncLabels(const std::vector<std::string>& labels) = 0;
    virtual void animateInsert(int value, size_t index) = 0;
    virtual void animateInsertString(const std::string& value, size_t index) = 0;
    virtual void animateRemove(size_t index) = 0;
    virtual void animateHighlight(size_t index) = 0;
    virtual void animateClear() = 0;
};
