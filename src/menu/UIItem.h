#ifndef OOP_UIITEM_H
#define OOP_UIITEM_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <set>
#include "world/WorldInterface.h"

class UIItem {
protected:
    std::set<sf::Event::EventType> interestingEvents;
    float marginTop = 2.5f, marginBottom = 2.5f;
    std::optional<sf::Vector2f> fixed_position;
    bool pixelAlign = false;
public:
    void setMargin(float marginTop, float marginBottom);
    [[nodiscard]] float getMarginTop() const;
    [[nodiscard]] float getMarginBottom() const;
    [[maybe_unused]] void setFixedPosition(std::optional<sf::Vector2f> position);
    [[nodiscard]] std::optional<sf::Vector2f> getFixedPosition() const;
    bool isMenuItem(); /// If it does not have a fixed position, it is a menu item.
    [[nodiscard]] const std::set<sf::Event::EventType> &getInterestingEvents() const;
    virtual SwitchCommand handleEvent(sf::Event &event);
    [[nodiscard]] virtual bool isAlwaysVisible() const;
    [[nodiscard]] virtual bool isSelectable() const;
    virtual void selected();
    virtual void deselected();
    virtual sf::Vector2f getLayoutSize() = 0; /// Only called for inline items.
    virtual void draw(sf::RenderWindow &window, sf::Vector2f position) = 0;
    void setPixelAlign(bool align);
    [[nodiscard]] bool getPixelAlign() const;
    virtual ~UIItem() = default;
};


#endif //OOP_UIITEM_H
