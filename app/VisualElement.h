#pragma once
#include "VisualComponent.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class VisualElement : public VisualComponent {
private:
	struct Animation {
		std::string type;
		float progress = 0.f;
		float duration = 1.f;
		sf::Vector2f startPos;
		sf::Vector2f targetPos;
		sf::Color startColor;
		sf::Color targetColor;
		bool active = false;
	};

	sf::Vector2f m_position;
	sf::Color m_color { sf::Color::White };
	sf::RectangleShape m_shape;
	sf::Text m_text;
	sf::Font& m_font;
	std::string m_name;
	std::vector<Animation> m_animations;

public:
	VisualElement(sf::Font& font, const std::string& text, const sf::Vector2f& position, const std::string& name="element")
		: m_position(position), m_font(font), m_name(name) {
		m_shape.setSize({100.f, 40.f});
		m_shape.setFillColor(sf::Color(30,30,80));
		m_shape.setOutlineColor(sf::Color::White);
		m_shape.setOutlineThickness(2.f);
		m_text.setFont(m_font);
		m_text.setString(text);
		m_text.setCharacterSize(20);
		m_text.setFillColor(sf::Color::White);
		setPosition(position);
	}

	void draw(sf::RenderWindow& window) const override {
		window.draw(m_shape);
		window.draw(m_text);
	}
	void update(float dt) override;
	void setPosition(const sf::Vector2f& pos) override;
	sf::Vector2f getPosition() const override { return m_position; }
	sf::FloatRect getBounds() const override { return m_shape.getGlobalBounds(); }

	void startAnimation(const std::string& type, float duration) override;
	bool hasActiveAnimations() const override;
	void setColor(const sf::Color& color) override { m_color = color; m_shape.setFillColor(color); }

	std::string getName() const override { return m_name; }
	void setName(const std::string& name) override { m_name = name; }
	const sf::Font& getFont() const { return m_font; }
};

class VectorElement : public VisualElement {
	int m_value;
	size_t m_index;
	static constexpr float BOX_W = 60.f;
	static constexpr float BOX_H = 60.f;
public:
	VectorElement(sf::Font& font, int value, size_t index, const sf::Vector2f& pos)
		: VisualElement(font, std::to_string(value), pos, "vector_element_"+std::to_string(index)), m_value(value), m_index(index) {
	}
	void setValue(int v) { m_value = v; setName("vector_element_"+std::to_string(m_index)); }
	int getValue() const { return m_value; }
	void setIndex(size_t i) { m_index = i; setName("vector_element_"+std::to_string(m_index)); }
	size_t getIndex() const { return m_index; }
};

class ListNodeElement : public VisualElement {
	int m_value;
	bool m_hasNext;
	static constexpr float NODE_W = 50.f;
	static constexpr float NODE_H = 30.f;
	static constexpr float PTR_W = 30.f;
public:
	ListNodeElement(sf::Font& font, int value, const sf::Vector2f& pos, bool hasNext)
		: VisualElement(font, std::to_string(value), pos, "list_node_"+std::to_string(value)), m_value(value), m_hasNext(hasNext) {}
	void setValue(int v) { m_value = v; }
	int getValue() const { return m_value; }
	void setHasNext(bool hn) { m_hasNext = hn; }
	bool getHasNext() const { return m_hasNext; }
	void drawConnection(sf::RenderWindow& window, const sf::Vector2f& targetPos) const;
};