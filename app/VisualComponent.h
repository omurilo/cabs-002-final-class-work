#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class VisualComponent {
public:
	virtual ~VisualComponent() = default;

	virtual void draw(sf::RenderWindow& window) const = 0;
	virtual void update(float dt) = 0;

	virtual void setPosition(const sf::Vector2f& pos) = 0;
	virtual sf::Vector2f getPosition() const = 0;
	virtual sf::FloatRect getBounds() const = 0;

	virtual void startAnimation(const std::string& type, float duration) = 0;
	virtual bool hasActiveAnimations() const = 0;
	virtual void setColor(const sf::Color& color) = 0;

	virtual void add(std::unique_ptr<VisualComponent>) {}
	virtual void remove(size_t) {}
	virtual VisualComponent* getChild(size_t) { return nullptr; }
	virtual size_t getChildCount() const { return 0; }
	virtual bool isComposite() const { return false; }

	virtual std::string getName() const = 0;
	virtual void setName(const std::string& name) = 0;
};