#include "menu/UIItem.h"

const std::set<sf::Event::EventType> &UIItem::getInterestingEvents() const {
    return interestingEvents;
}

WorldInterface::TickResult UIItem::handleEvent([[maybe_unused]] sf::Event &event) {
    return WorldInterface::CONTINUE();
}

bool UIItem::isAlwaysVisible() const {
    return false;
}

bool UIItem::isSelectable() const {
    return false;
}

void UIItem::selected() {}

void UIItem::deselected() {}

void UIItem::setMargin(float marginTop_, float marginBottom_) {
    marginTop = marginTop_;
    marginBottom = marginBottom_;
}

float UIItem::getMarginTop() const {
    return marginTop;
}

float UIItem::getMarginBottom() const {
    return marginBottom;
}

[[maybe_unused]] void UIItem::setFixedPosition(std::optional<sf::Vector2f> position) { // cppcheck-suppress unusedFunction
    fixed_position = position;
}

std::optional<sf::Vector2f> UIItem::getFixedPosition() const {
    return fixed_position;
}

bool UIItem::isMenuItem() {
    return !fixed_position.has_value();
}
