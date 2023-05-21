#include "UILabel.h"

#include <utility>

UILabel::UILabel(bool wantTicks) : UIItem(wantTicks) {}

UILabel::UILabel(sf::Text text, bool wantTicks) : UIItem(wantTicks), text(std::move(text)) {}

UILabel::UILabel(const sf::Font &font, bool wantTicks) : UIItem(wantTicks) {
    text.setFont(font);
}

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

void UILabel::draw(sf::RenderTarget &window, sf::Vector2f position) {
    text.setPosition(position);
    window.draw(text);
}

void UILabel::setString(const std::string &string) {
    setText(string);
}

[[maybe_unused]] void UILabel::setText(const sf::Text &text_) {
    this->text = text_;
}

void UILabel::setText(const std::string &text_) {
    this->text.setString(text_);
}

void UILabel::setFillColor(sf::Color color) {
    text.setFillColor(color);
}

void UILabel::setOutlineColor(sf::Color color) {
    text.setOutlineColor(color);
}

void UILabel::setCharacterSize(unsigned int size) {
    text.setCharacterSize(size);
}

void UILabel::setFont(const sf::Font &font) {
    text.setFont(font);
}

void UILabel::setStyle(sf::Uint32 style) {
    text.setStyle(style);
}
