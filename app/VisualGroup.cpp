#include "VisualGroup.h"
#include "VisualElement.h"
#include "AppStyle.h"
#include <cmath>
#include <algorithm>

VisualGroup::VisualGroup(const std::string& name, const sf::Vector2f& position)
    : m_position(position), m_name(name), m_backgroundColor(sf::Color::Transparent) {
}

void VisualGroup::draw(sf::RenderWindow& window) const {
    if (m_drawBackground) {
        sf::FloatRect bounds = getBounds();
        sf::RectangleShape background(sf::Vector2f(bounds.width + 20, bounds.height + 20));
        background.setPosition(bounds.left - 10, bounds.top - 10);
        background.setFillColor(m_backgroundColor);
        background.setOutlineColor(sf::Color(100, 100, 100));
        background.setOutlineThickness(1.0f);
        window.draw(background);
    }
    
    for (const auto& child : m_children) {
        child->draw(window);
    }
}

void VisualGroup::update(float deltaTime) {
    for (auto& child : m_children) {
        child->update(deltaTime);
    }
}

void VisualGroup::setPosition(const sf::Vector2f& position) {
    sf::Vector2f offset = position - m_position;
    m_position = position;
    
    for (auto& child : m_children) {
        child->setPosition(child->getPosition() + offset);
    }
}

sf::FloatRect VisualGroup::getBounds() const {
    if (m_children.empty()) {
        return sf::FloatRect(m_position.x, m_position.y, 0, 0);
    }
    
    sf::FloatRect bounds = m_children[0]->getBounds();
    for (size_t i = 1; i < m_children.size(); ++i) {
        sf::FloatRect childBounds = m_children[i]->getBounds();
        
        float left = std::min(bounds.left, childBounds.left);
        float top = std::min(bounds.top, childBounds.top);
        float right = std::max(bounds.left + bounds.width, childBounds.left + childBounds.width);
        float bottom = std::max(bounds.top + bounds.height, childBounds.top + childBounds.height);
        
        bounds = sf::FloatRect(left, top, right - left, bottom - top);
    }
    
    return bounds;
}

void VisualGroup::startAnimation(const std::string& type, float duration) {
    for (auto& child : m_children) {
        child->startAnimation(type, duration);
    }
}

bool VisualGroup::hasActiveAnimations() const {
    return std::any_of(m_children.begin(), m_children.end(),
                      [](const auto& child) { return child->hasActiveAnimations(); });
}

void VisualGroup::setColor(const sf::Color& color) {
    for (auto& child : m_children) {
        child->setColor(color);
    }
}

void VisualGroup::add(std::unique_ptr<VisualComponent> component) {
    if (component) {
        m_children.push_back(std::move(component));
    }
}

void VisualGroup::remove(size_t index) {
    if (index < m_children.size()) {
        m_children.erase(m_children.begin() + index);
    }
}

VisualComponent* VisualGroup::getChild(size_t index) {
    if (index < m_children.size()) {
        return m_children[index].get();
    }
    return nullptr;
}

void VisualGroup::arrangeHorizontally(float spacing) {
    float currentX = m_position.x;
    for (auto& child : m_children) {
        child->setPosition(sf::Vector2f(currentX, m_position.y));
        currentX += child->getBounds().width + spacing;
    }
}

void VisualGroup::arrangeVertically(float spacing) {
    float currentY = m_position.y;
    for (auto& child : m_children) {
        child->setPosition(sf::Vector2f(m_position.x, currentY));
        currentY += child->getBounds().height + spacing;
    }
}

void VisualGroup::arrangeInGrid(size_t columns, float spacing) {
    if (columns == 0) columns = 1;
    
    float startX = m_position.x;
    float startY = m_position.y;
    
    for (size_t i = 0; i < m_children.size(); ++i) {
        size_t row = i / columns;
        size_t col = i % columns;
        
        float x = startX + col * (60 + spacing);
        float y = startY + row * (40 + spacing);
        
        m_children[i]->setPosition(sf::Vector2f(x, y));
    }
}

void VisualGroup::arrangeInCircle(float radius) {
    if (m_children.empty()) return;
    
    float angleStep = 2 * M_PI / m_children.size();
    sf::Vector2f center = m_position;
    
    for (size_t i = 0; i < m_children.size(); ++i) {
        float angle = i * angleStep;
        float x = center.x + radius * std::cos(angle);
        float y = center.y + radius * std::sin(angle);
        m_children[i]->setPosition(sf::Vector2f(x, y));
    }
}

void VisualGroup::setBackgroundColor(const sf::Color& color, bool enabled) {
    m_backgroundColor = color;
    m_drawBackground = enabled;
}

void VisualGroup::clear() {
    m_children.clear();
}

VisualComponent* VisualGroup::findByName(const std::string& name) {
    for (auto& child : m_children) {
        if (child->getName() == name) {
            return child.get();
        }
        
        if (child->isComposite()) {
            auto* result = static_cast<VisualGroup*>(child.get())->findByName(name);
            if (result) return result;
        }
    }
    return nullptr;
}

void VisualGroup::forEach(std::function<void(VisualComponent*)> func) {
    for (auto& child : m_children) {
        func(child.get());
        if (child->isComposite()) {
            static_cast<VisualGroup*>(child.get())->forEach(func);
        }
    }
}

size_t VisualGroup::indexOf(VisualComponent* ptr) const {
    for (size_t i = 0; i < m_children.size(); ++i) {
        if (m_children[i].get() == ptr) return i;
    }
    return m_children.size();
}

VectorGroup::VectorGroup(const sf::Vector2f& position)
    : VisualGroup("VectorGroup", position) {
    setBackgroundColor(sf::Color(0, 0, 100, 50), true);
}

void VectorGroup::insertElement(int value, size_t index) {
    if (index > m_children.size()) index = m_children.size();
    
    static sf::Font defaultFont;
    static bool fontLoaded = false;
    if (!fontLoaded) {
        fontLoaded = defaultFont.loadFromFile("./arial.ttf") || defaultFont.loadFromFile("/System/Library/Fonts/Arial.ttf") ||
                    defaultFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    }
    
    auto element = std::make_unique<VectorElement>(defaultFont, value, index, sf::Vector2f(0, 0));
    
    m_children.insert(m_children.begin() + index, std::move(element));
    updateIndices();
    arrangeHorizontally(10.0f);
}

void VectorGroup::removeElement(size_t index) {
    if (index < m_children.size()) {
        remove(index);
        updateIndices();
        arrangeHorizontally(10.0f);
    }
}

void VectorGroup::updateIndices() {
    for (size_t i = 0; i < m_children.size(); ++i) {
        if (auto* vectorElement = dynamic_cast<VectorElement*>(m_children[i].get())) {
            vectorElement->setIndex(i);
        }
    }
}

void VectorGroup::setValue(size_t index, int value) {
    if (index < m_children.size()) {
        if (auto* vectorElement = dynamic_cast<VectorElement*>(m_children[index].get())) {
            vectorElement->setValue(value);
        }
    }
}

int VectorGroup::getValue(size_t index) const {
    if (index < m_children.size()) {
        if (const auto* vectorElement = dynamic_cast<const VectorElement*>(m_children[index].get())) {
            return vectorElement->getValue();
        }
    }
    return 0;
}

void VectorGroup::draw(sf::RenderWindow& window) const {
    VisualGroup::draw(window);
    
    static sf::Font defaultFont;
    static bool fontLoaded = false;
    if (!fontLoaded) {
        fontLoaded = defaultFont.loadFromFile("./arial.ttf") || defaultFont.loadFromFile("/System/Library/Fonts/Arial.ttf") ||
                    defaultFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    }
    
    if (fontLoaded) {
        sf::Text title("std::vector (Array List)", defaultFont, 24);
        title.setPosition(m_position.x, m_position.y - 50);
        title.setFillColor(sf::Color::White);
        window.draw(title);
    }
}

LinkedListGroup::LinkedListGroup(const sf::Vector2f& position)
    : VisualGroup("LinkedListGroup", position) {
    setBackgroundColor(sf::Color(100, 0, 0, 50), true);
}

void LinkedListGroup::insertNode(int value, size_t index) {
    if (index > m_children.size()) index = m_children.size();
    
    static sf::Font defaultFont;
    static bool fontLoaded = false;
    if (!fontLoaded) {
        fontLoaded = defaultFont.loadFromFile("./arial.ttf") || defaultFont.loadFromFile("/System/Library/Fonts/Arial.ttf") ||
                    defaultFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    }
    
    bool hasNext = (index < m_children.size());
    auto element = std::make_unique<ListNodeElement>(defaultFont, value, sf::Vector2f(0, 0), hasNext);
    
    m_children.insert(m_children.begin() + index, std::move(element));
    updateConnections();
    arrangeHorizontally(90.0f);
}

void LinkedListGroup::removeNode(size_t index) {
    if (index < m_children.size()) {
        remove(index);
        updateConnections();
        arrangeHorizontally(90.0f);
    }
}

void LinkedListGroup::updateConnections() {
    for (size_t i = 0; i < m_children.size(); ++i) {
        if (auto* listNode = dynamic_cast<ListNodeElement*>(m_children[i].get())) {
            listNode->setHasNext(i < m_children.size() - 1);
        }
    }
}

void LinkedListGroup::setValue(size_t index, int value) {
    if (index < m_children.size()) {
        if (auto* listNode = dynamic_cast<ListNodeElement*>(m_children[index].get())) {
            listNode->setValue(value);
        }
    }
}

int LinkedListGroup::getValue(size_t index) const {
    if (index < m_children.size()) {
        if (const auto* listNode = dynamic_cast<const ListNodeElement*>(m_children[index].get())) {
            return listNode->getValue();
        }
    }
    return 0;
}

void LinkedListGroup::draw(sf::RenderWindow& window) const {
    VisualGroup::draw(window);
    drawConnections(window);
    
    static sf::Font defaultFont;
    static bool fontLoaded = false;
    if (!fontLoaded) {
        fontLoaded = defaultFont.loadFromFile("./arial.ttf") || defaultFont.loadFromFile("/System/Library/Fonts/Arial.ttf") ||
                    defaultFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    }
    
    if (fontLoaded) {
        sf::Text title("std::list (Linked List)", defaultFont, 24);
        title.setPosition(m_position.x, m_position.y - 50);
        title.setFillColor(sf::Color::White);
        window.draw(title);
        
        if (!m_children.empty()) {
            sf::Text headText("head", defaultFont, 18);
            headText.setFillColor(sf::Color::Yellow);
            sf::Vector2f firstNodePos = m_children[0]->getPosition();
            headText.setPosition(firstNodePos.x, firstNodePos.y - 30);
            window.draw(headText);
        }
    }
}

void LinkedListGroup::drawConnections(sf::RenderWindow& window) const {
    for (size_t i = 0; i < m_children.size() - 1; ++i) {
        if (const auto* currentNode = dynamic_cast<const ListNodeElement*>(m_children[i].get())) {
            sf::Vector2f nextNodePos = m_children[i + 1]->getPosition();
            currentNode->drawConnection(window, nextNodePos);
        }
    }
}