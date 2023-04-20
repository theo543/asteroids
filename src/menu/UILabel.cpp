#include "UILabel.h"

#include <utility>

UILabel::UILabel(sf::Text text) : text(std::move(text)) {}

UILabel::UILabel(const std::string &text, const sf::Font &font, unsigned int characterSize,
                 sf::Color textColor, sf::Color fillColor) {
    this->text.setString(text);
    this->text.setFont(font);
    this->text.setFillColor(fillColor);
    this->text.setOutlineColor(textColor);
    this->text.setCharacterSize(characterSize);
}

sf::Vector2f UILabel::getLayoutSize() {
    return text.getGlobalBounds().getSize();
}

void UILabel::draw(sf::RenderWindow &window, sf::Vector2f position) {
    text.setPosition(position);
    window.draw(text);
}

[[maybe_unused]] void UILabel::setText(const sf::Text &text_) { // cppcheck-suppress unusedFunction
    this->text = text_;
}
