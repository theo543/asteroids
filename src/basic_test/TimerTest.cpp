#include <fmt/core.h>
#include <fmt/chrono.h>
#include <chrono>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Err.hpp>
#include "basic_test/TimerTest.h"
#include "menu/UILabel.h"
#include "menu/Stats.h"
#include "resources/GlobalLoaders.h"

TimerTest::TimerTest() : WorldBase(true), testSound(std::make_shared<sf::Sound>()), timeText(std::make_shared<UILabel>()) {
    sf::err()<<"TimerTest constructor called at UTC "<<fmt::format("{:%H:%M:%S}", std::chrono::system_clock::now().time_since_epoch())<<std::endl;
    setTiming(sf::seconds(static_cast<float>(1.0L / 120.0L)), 16);
    const auto &pixel = GlobalLoaders::Fonts().load("PublicPixelTTF");
    timeText->setFont(pixel);
    timeText->setCharacterSize(48);
    timeText->setStyle(sf::Text::Bold);
    testSound->setBuffer(GlobalLoaders::SoundBuffers().load("PianoLoopsOGG"));
    testSound->setLoop(true);
    testSound->play();
    ui.setExitHandler([this](){
        if(areYouSure) {
            return SwitchFactory::pop();
        } else {
            areYouSure = true;
            exitTimer.restart();
            return SwitchFactory::empty();
        }
    });
    ui.setHideBehavior(UI::HideBehavior::Exit);
    ui.addItem(std::make_shared<Stats>(pixel, testSound));
    timeText->setPixelAlign(true);
    ui.addItem(timeText);
}

void TimerTest::onLoadWorld(sf::RenderWindow &window) {
    window.setTitle("SFML works!");
}

void TimerTest::drawWorld(sf::RenderWindow&) {}

SwitchCommand TimerTest::tickWorld() {
    displayTime += getTimePerTick();
    updateTime();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        sf::sleep(sf::seconds(1.0f)); // Simulate lag
    }
    return SwitchFactory::empty();
}

const sf::Time TimerTest::exitDelay = sf::seconds(3);
void TimerTest::updateTime() {
    if(areYouSure) {
        if(exitTimer.getElapsedTime() >= exitDelay) {
            areYouSure = false;
        } else {
            timeText->setFillColor(sf::Color::Red);
            timeText->setString("Confirm exit?");
            return;
        }
    }
    timeText->setFillColor(sf::Color::White);
    auto time = static_cast<unsigned int>(displayTime.asSeconds());
    auto hours = time / 3600;
    auto minutes = (time % 3600) / 60;
    auto seconds = time % 60;
    // use fmt to print the time without decimal places
    timeText->setString(fmt::format("{:02}:{:02}:{:02}", hours, minutes, seconds));
}

TimerTest::~TimerTest() {
    sf::err()<<"TimerTest destructor called at UTC "<<fmt::format("{:%H:%M:%S}", std::chrono::system_clock::now().time_since_epoch())<<std::endl;
    testSound->stop();
}
