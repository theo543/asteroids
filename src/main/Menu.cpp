#include "main/Menu.h"
#include "menu/UILabel.h"
#include "menu/UIOption.h"
#include "menu/RainbowBackground.h"
#include "resources/GlobalLoaders.h"
#include "basic_test/TimerTest.h"
#include "bounce_test/BounceTest.h"
#include "world/type_not_of_claimed_base.h"
#include "physics/CollisionTest.h"
#include "asteroids/AsteroidsWorld.h"

Menu::Menu() {
    setBackgroundColor(sf::Color::White);
    ui.addItem(std::make_shared<RainbowBackground>());
    const auto &f = GlobalLoaders::Fonts().load("PublicPixelTTF");
    auto label1 = std::make_shared<UILabel>("Tests:", f, 36, sf::Color::White, sf::Color::Black);
    label1->setMargin(0.f, 96.f);
    ui.addItem(label1);
    auto Gray = sf::Color(128, 128, 128);
    auto unselected_style = UIOption::Fill_Outline{Gray, sf::Color::Transparent};
    auto selected_style = UIOption::Fill_Outline {Gray, sf::Color::Black};
    auto asteroidsOpt = std::make_shared<UIOption>("Asteroids", f, unselected_style, selected_style, 24, []() {
        return SwitchFactory::push<AsteroidsWorld>();
    });
    auto clockOpt = std::make_shared<UIOption>("Clock", f, unselected_style, selected_style, 24, []() {
        return SwitchFactory::push<TimerTest>();
    });
    auto bounceOpt = std::make_shared<UIOption>("Bouncy", f, unselected_style, selected_style, 24, []() {
        return SwitchFactory::push<BounceTest>();
    });
    auto collisionOpt = std::make_shared<UIOption>("Collision", f, unselected_style, selected_style, 24, []() {
        return SwitchFactory::push<CollisionTest>();
    });
    auto unhandledTest = std::make_shared<UIOption>("Unhandled Exception Test", f, unselected_style, selected_style, 24, []() {
        SwitchFactory::push<TimerTest>(std::unique_ptr<WorldInterface>(new BounceTest()));
        return SwitchFactory::empty();
    });
    auto exceptionTest = std::make_shared<UIOption>("Exception Test", f, unselected_style, selected_style, 24, [](UIOption &exceptionTest) {
        auto success_unselected_style = UIOption::Fill_Outline{sf::Color::Green, sf::Color::Transparent};
        auto success_selected_style = UIOption::Fill_Outline {sf::Color::Green, sf::Color::Black};
        auto darkRed = sf::Color(168, 0, 0);
        auto failure_unselected_style = UIOption::Fill_Outline{darkRed, sf::Color::Transparent};
        auto failure_selected_style = UIOption::Fill_Outline {darkRed, sf::Color::Black};
        bool success = false;
        try {
            SwitchFactory::push<TimerTest>(std::unique_ptr<WorldInterface>(new BounceTest()));
        } catch (const type_not_of_claimed_base &err) {
            success = true;
            sf::err() << "Correct exception caught! \nwhat(): " << std::string(err.what()) << std::endl;
        } catch(...) {
            sf::err() << "Wrong exception caught!" << std::endl;
        }
        if(success) {
            exceptionTest.setSelectedColor(success_selected_style);
            exceptionTest.setUnselectedColor(success_unselected_style);
        } else {
            exceptionTest.setSelectedColor(failure_selected_style);
            exceptionTest.setUnselectedColor(failure_unselected_style);
        }
        return SwitchFactory::empty();
    });
    auto toggleFullscreen = std::make_shared<UIOption>("Toggle Fullscreen", f, unselected_style, selected_style, 24, []() {
        return SwitchFactory::fullscreen_toggle();
    });
    unselected_style = UIOption::Fill_Outline{sf::Color::Red, sf::Color::Transparent};
    selected_style = UIOption::Fill_Outline {sf::Color::Red, sf::Color::Black};
    auto exitOpt = std::make_shared<UIOption>("Exit", f, unselected_style, selected_style, 24, []() {
        return SwitchFactory::pop();
    });
    ui.addItem(asteroidsOpt);
    ui.addItem(clockOpt);
    ui.addItem(bounceOpt);
    ui.addItem(collisionOpt);
    ui.addItem(unhandledTest);
    ui.addItem(exceptionTest);
    ui.addItem(toggleFullscreen);
    ui.addItem(exitOpt);
    ui.setHideBehavior(UI::HideBehavior::Exit);
    ui.forAllSetPixelAlign(true);
}

void Menu::onLoadWorld(sf::RenderWindow &window) {
    window.setTitle("Demo");
}

void Menu::drawWorld(sf::RenderWindow&) {}

SwitchCommand Menu::tickWorld() {
    return SwitchFactory::empty();
}
