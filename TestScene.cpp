#include "TestScene.h"
#include "embedded_fwd.h"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <iostream>

TestScene::TestScene() {
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
}

void TestScene::draw(sf::RenderWindow &window) {
    float x = window.getView().getSize().x / 2.0f - timeText.getGlobalBounds().width / 2.0f;
    float y = window.getView().getSize().y / 2.0f - timeText.getGlobalBounds().height / 2.0f - 2; // Compensate for weird font baseline
    timeText.setPosition(x, y);
    window.draw(timeText);
    window.draw(statsText);
    frames++;
}

bool TestScene::handleEvent(sf::Event &event) {
    if(event.type == sf::Event::Closed) {
        if(areYouSure) {
            return true;
        } else {
            areYouSure = true;
            exitTimer.restart();
            return false;
        }
    }
    return true;
}

Scene::TickResult TestScene::tick()  {
    ticks++;
    updateStats();
    updateTime();
    return std::nullopt;
}

void TestScene::updateStats() {
    statsText.setString(
            "FPS: " + std::to_string(fps) +
            "\nTPS: " + std::to_string(tps) +
            "\nF: " + std::to_string(frames) +
            "\nT: " + std::to_string(ticks));
    if(sinceStatsReset.getElapsedTime() >= sf::seconds(1)) {
        sinceStatsReset.restart();
        fps = frames;
        tps = ticks;
        frames = 0;
        ticks = 0;
    }
}

const sf::Time TestScene::exitDelay = sf::seconds(3);
void TestScene::updateTime() {
    if(areYouSure) {
        if(exitTimer.getElapsedTime() >= exitDelay) {
            areYouSure = false;
        } else {
            timeText.setFillColor(sf::Color::Red);
            timeText.setString("Confirm exit?");
            return;
        }
    }
    timeText.setFillColor(sf::Color::White);
    std::time_t t = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "[%H : %M : %S]");
    timeText.setString(ss.str());
}
