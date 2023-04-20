#ifndef OOP_UILABEL_H
#define OOP_UILABEL_H

#include "menu/UIItem.h"
#include <optional>

class UILabel : public UIItem {
    sf::Text text;
public:
    explicit UILabel(sf::Text text);
    UILabel(const std::string &text, const sf::Font &font, unsigned int characterSize,
            sf::Color textColor = sf::Color::Black,
            sf::Color fillColor = sf::Color::Transparent);
    sf::Vector2f getLayoutSize() override;
    void draw(sf::RenderWindow &window, sf::Vector2f position) override;
    [[maybe_unused]] void setText(const sf::Text &text);
};


#endif //OOP_UILABEL_H
