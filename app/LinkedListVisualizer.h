#pragma once
#include "Visualizer.h"
#include "LayoutPolicy.h"

class LinkedListVisualizer final : public Visualizer {
public:
    LinkedListVisualizer(sf::Font& font, const sf::Vector2f& position);

    void push_front(int value);
    void pop_front();
    void insertAt(int value, size_t index);
         void clearAnimated();
    void push_front_string(const std::string& label);
    void insertAtString(const std::string& label, size_t index);
    
    void draw(sf::RenderWindow& window) const override;
    void reflow(float windowWidth, float panelWidth = 280.f);

    void animateInsert(int value, size_t index) override { insertAt(value, index); }
    void animateInsertString(const std::string& value, size_t index) override { insertAtString(value, index); }
    void animateRemove(size_t index) override { if (index==0) pop_front();  }
    void animateHighlight(size_t index) override { highlight(index); }
    void animateClear() override { clearAnimated(); }

private:
    void buildPushFrontAnimation(int value);
    void buildPopFrontAnimation();
    void buildInsertAtAnimation(int value, size_t index);
    void buildPushFrontStringAnimation(const std::string& label);
    void buildInsertAtStringAnimation(const std::string& label, size_t index);

    sf::Vector2f getPositionForIndex(size_t i);

    sf::Font& m_font;
    sf::Vector2f m_position;
    std::unique_ptr<ILayoutPolicy> m_layout;
    static constexpr float NODE_WIDTH = 84.f;
    static constexpr float NODE_HEIGHT = 60.f;
    static constexpr float PTR_WIDTH = 30.f;
    static constexpr float SPACING = 70.f;
    static constexpr int FONT_SIZE = 32;
    float m_lastLayoutWidth = 0.f;
    size_t m_lastNodeCount = 0;
    void enqueueStringInsert(const std::string& label, size_t index) {
        std::string desc = "Lista: Insert(\"" + label + "\", " + std::to_string(index) + ")";
        enqueueOperation(desc, [this, label, index]() {
            if (index==0) buildPushFrontStringAnimation(label); else buildInsertAtStringAnimation(label, index);
        });
    }
};
