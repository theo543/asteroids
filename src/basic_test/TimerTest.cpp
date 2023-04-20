#include "TimerTest.h"
#include "../embedded_fwd.h"
#include <iostream>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <chrono>

TimerTest::TimerTest() : WorldBase(true), testSound(std::make_shared<sf::Sound>()), timeText(std::make_shared<UILabel>()) {
    std::cout<<"TimerTest constructor called at UTC "<<fmt::format("{:%H:%M:%S}", std::chrono::system_clock::now().time_since_epoch())<<std::endl;
    setTiming(sf::seconds(static_cast<float>(1.0L / 120.0L)), 16);
    font.loadFromMemory(PublicPixelTTF.data(), PublicPixelTTF.size());
    buffer.loadFromMemory(TestSoundOGG.data(), TestSoundOGG.size());
    stats = std::make_shared<Stats>(font, testSound);
    timeText->setFont(font);
    timeText->setCharacterSize(48);
    timeText->setStyle(sf::Text::Bold);
    testSound->setBuffer(buffer);
    testSound->setLoop(true);
    testSound->play();
    ui.setExitHandler([this](){
        if(areYouSure) {
            return WorldInterface::EXIT();
        } else {
            areYouSure = true;
            exitTimer.restart();
            return WorldInterface::CONTINUE();
        }
    });
    ui.addItem(stats);
    ui.addItem(timeText);
}

void TimerTest::initWorld(sf::RenderWindow &window) {
    window.setTitle("SFML works!");
}

void TimerTest::drawWorld([[maybe_unused]] sf::RenderWindow &window) {}

WorldInterface::TickResult TimerTest::tickWorld() {
    stats->tickOccurred();
    displayTime += getTimePerTick();
    updateTime();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        sf::sleep(sf::seconds(1.0f)); // Simulate lag
    }
    return WorldInterface::CONTINUE();
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
    std::cout<<"TimerTest destructor called at UTC "<<fmt::format("{:%H:%M:%S}", std::chrono::system_clock::now().time_since_epoch())<<std::endl;
    testSound->stop();
}
