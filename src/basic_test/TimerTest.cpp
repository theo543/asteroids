#include "TimerTest.h"
#include "../embedded_fwd.h"
#include <iostream>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <chrono>

TimerTest::TimerTest() : WorldInterface(true) {
    std::cout<<"TimerTest constructor called at UTC "<<fmt::format("{:%H:%M:%S}", std::chrono::system_clock::now().time_since_epoch())<<std::endl;
    setTiming(sf::seconds(static_cast<float>(1.0L / 120.0L)), 16);
    sinceStatsReset.restart();
    font.loadFromMemory(PublicPixelTTF.data(), PublicPixelTTF.size());
    timeText.setFont(font);
    timeText.setCharacterSize(48); // in pixels, not points!
    timeText.setFillColor(sf::Color::White);
    timeText.setStyle(sf::Text::Bold);
    timeText.setOutlineColor(sf::Color::Black);
    timeText.setOutlineThickness(2);
    statsText.setFont(font);
    statsText.setCharacterSize(24); // in pixels, not points!
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(0, 0);
    buffer.loadFromMemory(TestSoundOGG.data(), TestSoundOGG.size());
    testSound.setBuffer(buffer);
    testSound.setLoop(true);
    testSound.play();
}

void TimerTest::init(sf::RenderWindow &window) {
    window.setTitle("SFML works!");
}

void TimerTest::draw(sf::RenderWindow &window) {
    float x = window.getView().getSize().x / 2.0f - timeText.getGlobalBounds().width / 2.0f;
    float y = window.getView().getSize().y / 2.0f - timeText.getGlobalBounds().height / 2.0f - 2; // Compensate for weird font baseline
    timeText.setPosition(x, y);
    window.draw(timeText);
    window.draw(statsText);
    frames++;
}

void TimerTest::handleEvent(sf::Event &event) {
    if(event.type == sf::Event::Closed) {
        if(areYouSure) {
            exitConfirmed = true;
        } else {
            areYouSure = true;
            std::lock_guard<std::mutex> lock(exitTimerMutex);
            exitTimer.restart();
        }
    }
}

WorldInterface::TickResult TimerTest::tick() {
    ticks++;
    displayTime += getTimePerTick();
    updateStats();
    updateTime();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        sf::sleep(sf::seconds(1.0f)); // Simulate lag
    }
    if (exitConfirmed) return {nullptr};
    return std::nullopt;
}

void TimerTest::updateStats() {
    statsText.setString(
            "FPS: " + std::to_string(fps) +
            "\nTPS: " + std::to_string(tps) +
            "\nF: " + std::to_string(frames) +
            "\nT: " + std::to_string(ticks) +
            "\nSound test: " + fmt::format("{:%H:%M:%S}", std::chrono::duration<float>(testSound.getPlayingOffset().asSeconds())));
    if(sinceStatsReset.getElapsedTime() >= sf::seconds(1)) {
        sinceStatsReset.restart();
        fps = frames;
        tps = ticks;
        frames = 0;
        ticks = 0;
    }
}

const sf::Time TimerTest::exitDelay = sf::seconds(3);
void TimerTest::updateTime() {
    if(areYouSure) {
        std::lock_guard<std::mutex> lock(exitTimerMutex);
        if(exitTimer.getElapsedTime() >= exitDelay) {
            areYouSure = false;
        } else {
            timeText.setFillColor(sf::Color::Red);
            timeText.setString("Confirm exit?");
            return;
        }
    }
    timeText.setFillColor(sf::Color::White);
    auto time = static_cast<unsigned int>(displayTime.asSeconds());
    auto hours = time / 3600;
    auto minutes = (time % 3600) / 60;
    auto seconds = time % 60;
    // use fmt to print the time without decimal places
    timeText.setString(fmt::format("{:02}:{:02}:{:02}", hours, minutes, seconds));
}

TimerTest::~TimerTest() {
    std::cout<<"TimerTest destructor called at UTC "<<fmt::format("{:%H:%M:%S}", std::chrono::system_clock::now().time_since_epoch())<<std::endl;
    testSound.stop();
}
