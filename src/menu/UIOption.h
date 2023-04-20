#ifndef OOP_UIOPTION_H
#define OOP_UIOPTION_H

#include "menu/UIItem.h"
#include <functional>

class UIOption : public UIItem {
public:
    typedef std::pair<sf::Color, sf::Color> FillAndTextColor;
private:
    FillAndTextColor unselectedColor, selectedColor;
    sf::Text text;
    std::function<WorldInterface::TickResult()> callback;
    bool selectedState = false;
public:
    UIOption(const std::string &text, const sf::Font &font, FillAndTextColor unselected, FillAndTextColor selected,
             unsigned int characterSize, std::function<WorldInterface::TickResult()> callback);
    sf::Vector2f getLayoutSize() override;
    void draw(sf::RenderWindow &window, sf::Vector2f position) override;
    void selected() override;
    void deselected() override;
    bool isSelectable() const override;
    WorldInterface::TickResult handleEvent(sf::Event &event) override;
};


#endif //OOP_UIOPTION_H
