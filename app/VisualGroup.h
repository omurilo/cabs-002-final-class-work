#pragma once
#include "VisualComponent.h"
#include "VisualElement.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>

class VisualGroup : public VisualComponent {
protected:
	std::vector<std::unique_ptr<VisualComponent>> m_children;
	sf::Vector2f m_position;
	std::string m_name;
	sf::Color m_backgroundColor;
	bool m_drawBackground = false;
public:
	VisualGroup(const std::string& name, const sf::Vector2f& position);
	virtual ~VisualGroup() = default;

	void draw(sf::RenderWindow& window) const override;
	void update(float dt) override;
	void setPosition(const sf::Vector2f& position) override;
	sf::Vector2f getPosition() const override { return m_position; }
	sf::FloatRect getBounds() const override;

	void startAnimation(const std::string& type, float duration) override;
	bool hasActiveAnimations() const override;
	void setColor(const sf::Color& color) override;

	std::string getName() const override { return m_name; }
	void setName(const std::string& name) override { m_name = name; }

	void add(std::unique_ptr<VisualComponent> component) override;
	void remove(size_t index) override;
	VisualComponent* getChild(size_t index) override;
	size_t getChildCount() const override { return m_children.size(); }
	bool isComposite() const override { return true; }

	void arrangeHorizontally(float spacing = 10.0f);
	void arrangeVertically(float spacing = 10.0f);
	void arrangeInGrid(size_t columns, float spacing = 10.0f);
	void arrangeInCircle(float radius);

	void setBackgroundColor(const sf::Color& color, bool enabled = true);
	void setDrawBackground(bool enabled) { m_drawBackground = enabled; }
	void clear();
	VisualComponent* findByName(const std::string& name);
	void forEach(std::function<void(VisualComponent*)> func);
	size_t indexOf(VisualComponent* ptr) const;
};

class VectorGroup : public VisualGroup {
public:
	VectorGroup(const sf::Vector2f& position);
	void insertElement(int value, size_t index);
	void removeElement(size_t index);
	void updateIndices();
	void setValue(size_t index, int value);
	int getValue(size_t index) const;
	void draw(sf::RenderWindow& window) const override;
};

class LinkedListGroup : public VisualGroup {
public:
	LinkedListGroup(const sf::Vector2f& position);
	void insertNode(int value, size_t index);
	void removeNode(size_t index);
	void updateConnections();
	void setValue(size_t index, int value);
	int getValue(size_t index) const;
	void draw(sf::RenderWindow& window) const override;
private:
	void drawConnections(sf::RenderWindow& window) const;
};