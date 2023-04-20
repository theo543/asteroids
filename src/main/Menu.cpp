#include "main/Menu.h"
#include "menu/UILabel.h"
#include "menu/UIOption.h"
#include "../embedded_fwd.h"
#include "basic_test/TimerTest.h"
#include "bounce_test/BounceTest.h"

void Menu::initWorld([[maybe_unused]] sf::RenderWindow  &window) {
    window.setTitle("Demo");
    setBackgroundColor(sf::Color::White);
    f.loadFromMemory(PublicPixelTTF.data(), PublicPixelTTF.size());
    auto label1 = std::make_shared<UILabel>("Tests:", f, 36, sf::Color::White, sf::Color::Black);
    label1->setMargin(0.f, 96.f);
    ui.addItem(label1);
    auto selected_style = std::pair{sf::Color::White, sf::Color::Black};
    auto unselected_style = std::pair{sf::Color::Black, sf::Color::White};
    auto clockOpt = std::make_shared<UIOption>("Clock", f, selected_style, unselected_style, 24, []() {
        return std::make_unique<TimerTest>();
    });
    auto bounceOpt = std::make_shared<UIOption>("Bouncy", f, selected_style, unselected_style, 24, []() {
        return std::make_unique<BounceTest>();
    });
    ui.addItem(clockOpt);
    ui.addItem(bounceOpt);
    ui.setHideBehavior(UI::HideBehavior::Exit);
}

void Menu::drawWorld([[maybe_unused]] sf::RenderWindow &window) {}

WorldInterface::TickResult Menu::tickWorld() {
    return WorldInterface::CONTINUE();
}
