#include "UIOption.h"
#include "world/SwitchFactory.h"

#include <utility>

UIOption::UIOption(const std::string &text, const sf::Font &font, Fill_Outline unselected, Fill_Outline selected,
                   unsigned int characterSize, std::function<SwitchCommand(UIOption &)> callback, float outlineThickness)
 : UILabel(text, font, characterSize),
 unselectedColor(std::move(unselected)), selectedColor(std::move(selected)), callback(std::move(callback)), selectedState(false) {
    this->text.setOutlineThickness(outlineThickness);
    interestingEvents.insert(sf::Event::KeyPressed);
}

void UIOption::draw(sf::RenderTarget &window, sf::Vector2f position) {
    auto colors = (selectedState ? selectedColor : unselectedColor);
    text.setFillColor(colors.first);
    text.setOutlineColor(colors.second);
    UILabel::draw(window, position);
}

void UIOption::selected() {
    selectedState = true;
    UILabel::selected();
}

void UIOption::deselected() {
    selectedState = false;
    UILabel::deselected();
}

bool UIOption::isSelectable() const {
    return true;
}

SwitchCommand UIOption::handleEvent(sf::Event &event) {
    if(selectedState && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return
    && callback != nullptr) {
        return callback(*this);
    }
    return UILabel::handleEvent(event);
}

void UIOption::setSelectedColor(UIOption::Fill_Outline fo) {
    selectedColor = fo;
}

void UIOption::setUnselectedColor(UIOption::Fill_Outline fo) {
    unselectedColor = fo;
}

UIOption::UIOption(const std::string &text, const sf::Font &font, UIOption::Fill_Outline unselected,
                   UIOption::Fill_Outline selected, unsigned int characterSize, const std::function<SwitchCommand()>& callback,
                   float outlineThickness)
                   : UIOption(text, font, unselected, selected, characterSize, [callback](UIItem&){return callback();}, outlineThickness)
                   {}
