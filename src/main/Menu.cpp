#include "main/Menu.h"
#include "menu/UILabel.h"
#include "menu/UIOption.h"
#include "../embedded_fwd.h"
#include "basic_test/TimerTest.h"
#include "bounce_test/BounceTest.h"

Menu::Menu() {
    setBackgroundColor(sf::Color::White);
    f.loadFromMemory(PublicPixelTTF.data(), PublicPixelTTF.size());
    auto label1 = std::make_shared<UILabel>("Tests:", f, 36, sf::Color::White, sf::Color::Black);
    label1->setMargin(0.f, 96.f);
    ui.addItem(label1);
    auto Gray = sf::Color(128, 128, 128);
    auto selected_style = UIOption::Fill_Outline{Gray, sf::Color::Transparent};
    auto unselected_style = UIOption::Fill_Outline {Gray, sf::Color::Black};
    auto clockOpt = std::make_shared<UIOption>("Clock", f, selected_style, unselected_style, 24, []() {
        return SwitchFactory::push<TimerTest>();
    });
    auto bounceOpt = std::make_shared<UIOption>("Bouncy", f, selected_style, unselected_style, 24, []() {
        return SwitchFactory::push<BounceTest>();
    });
    selected_style = UIOption::Fill_Outline{sf::Color::Red, sf::Color::Transparent};
    unselected_style = UIOption::Fill_Outline {sf::Color::Red, sf::Color::Black};
    auto exitOpt = std::make_shared<UIOption>("Exit", f, selected_style, unselected_style, 24, []() {
        return SwitchFactory::pop();
    });
    ui.addItem(clockOpt);
    ui.addItem(bounceOpt);
    ui.addItem(exitOpt);
    ui.setHideBehavior(UI::HideBehavior::Exit);
    ui.forAllSetPixelAlign(true);
}

void Menu::initWorld(sf::RenderWindow &window) {
    window.setTitle("Demo");
}

void Menu::drawWorld(sf::RenderWindow&) {}

SwitchCommand Menu::tickWorld() {
    return SwitchFactory::empty();
}
