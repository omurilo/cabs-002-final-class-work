#include "VisualElement.h"
#include <algorithm>
#include <cmath>

void VisualElement::update(float dt) {
	for (auto& anim : m_animations) {
		if (!anim.active) continue;
		anim.progress += dt / anim.duration;
		if (anim.progress >= 1.f) { anim.progress = 1.f; anim.active = false; }
		float t = anim.progress;
		t = t * t * (3.f - 2.f * t);
		if (anim.type == "move") {
			sf::Vector2f pos = anim.startPos + (anim.targetPos - anim.startPos) * t;
			setPosition(pos);
		} else if (anim.type == "color") {
			sf::Uint8 r = static_cast<sf::Uint8>(anim.startColor.r + (anim.targetColor.r - anim.startColor.r) * t);
			sf::Uint8 g = static_cast<sf::Uint8>(anim.startColor.g + (anim.targetColor.g - anim.startColor.g) * t);
			sf::Uint8 b = static_cast<sf::Uint8>(anim.startColor.b + (anim.targetColor.b - anim.startColor.b) * t);
			setColor(sf::Color(r,g,b));
		}
	}
}

void VisualElement::setPosition(const sf::Vector2f& pos) {
	m_position = pos;
	m_shape.setPosition(pos);
	sf::FloatRect tb = m_text.getLocalBounds();
	sf::Vector2f sz = m_shape.getSize();
	m_text.setPosition(pos.x + (sz.x - tb.width)/2.f - tb.left, pos.y + (sz.y - tb.height)/2.f - tb.top);
}

void VisualElement::startAnimation(const std::string& type, float duration) {
	Animation anim;
	anim.type = (type == "move" ? "move" : (type=="color"?"color":"color"));
	anim.duration = duration;
	anim.progress = 0.f;
	anim.active = true;
	anim.startPos = m_position;
	anim.targetPos = m_position;
	anim.startColor = m_color;
	anim.targetColor = (type=="highlight"? sf::Color::Yellow : (type=="remove"? sf::Color::Red : sf::Color::Cyan));
	m_animations.push_back(anim);
}

bool VisualElement::hasActiveAnimations() const {
	return std::any_of(m_animations.begin(), m_animations.end(), [](const Animation& a){ return a.active; });
}

void ListNodeElement::drawConnection(sf::RenderWindow& window, const sf::Vector2f& targetPos) const {
	if (!m_hasNext) return;
	sf::Vector2f pos = getPosition();
	sf::Vector2f startPos(pos.x + NODE_W + PTR_W / 2.f, pos.y + NODE_H / 2.f);
	sf::Vector2f endPos(targetPos.x, targetPos.y + NODE_H / 2.f);
	sf::Vertex line[] = { sf::Vertex(startPos, sf::Color::Red), sf::Vertex(endPos, sf::Color::Red) };
	window.draw(line, 2, sf::Lines);
}