
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "IModel.h"

class IView {
public:
    virtual ~IView() = default;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual void syncState(const DataState& state) = 0; 
    virtual void animateInsert(int value, size_t index) = 0;
    virtual void animateInsertString(const std::string& value, size_t index) = 0;
    virtual void animateRemove(size_t index) = 0;
    virtual void animateHighlight(size_t index) = 0;
    virtual void animateClear() = 0;
};
