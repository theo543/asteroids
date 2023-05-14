#include "menu/UIItem.h"
#include "world/SwitchFactory.h"

const std::set<sf::Event::EventType> &UIItem::getInterestingEvents() const {
    return interestingEvents;
}

SwitchCommand UIItem::handleEvent(sf::Event&) {
    return SwitchFactory::empty();
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

void UIItem::setPixelAlign(bool align) {
    pixelAlign = align;
}

bool UIItem::getPixelAlign() const {
    return pixelAlign;
}
