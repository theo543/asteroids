#ifndef OOP_UIOPTION_H
#define OOP_UIOPTION_H
#include "menu/UILabel.h"
#include <functional>

class UIOption : public UILabel {
public:
    typedef std::pair<sf::Color, sf::Color> Fill_Outline;
private:
    Fill_Outline unselectedColor, selectedColor;
    std::function<SwitchCommand(UIOption&)> callback;
    bool selectedState = false;
public:
    UIOption(const std::string &text, const sf::Font &font, Fill_Outline unselected, Fill_Outline selected,
             unsigned int characterSize, std::function<SwitchCommand(UIOption&)> callback, float outlineThickness = 2.f);
    UIOption(const std::string &text, const sf::Font &font, Fill_Outline unselected, Fill_Outline selected,
             unsigned int characterSize, const std::function<SwitchCommand()>& callback, float outlineThickness = 2.f);
    void draw(sf::RenderTarget &window, sf::Vector2f position) override;
    void selected() override;
    void deselected() override;
    bool isSelectable() const override;
    void setSelectedColor(Fill_Outline);
    void setUnselectedColor(Fill_Outline);
    SwitchCommand handleEvent(sf::Event &event) override;
};


#endif //OOP_UIOPTION_H
