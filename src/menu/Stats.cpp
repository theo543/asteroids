#include <fmt/chrono.h>
#include <chrono>
#include "menu/Stats.h"

Stats::Stats(const sf::Font &font, std::shared_ptr<sf::Sound> testSound) : UILabel(font, true), testSound(std::move(testSound)) {
    setCharacterSize(24);
    setOutlineColor(sf::Color::White);
    setFixedPosition(sf::Vector2f{0, 0}); // Fixed by default
}

void Stats::notifyTick() {
    ticks++;
    if (sinceStatsReset.getElapsedTime().asSeconds() >= 1) {
        fps = frames;
        tps = ticks;
        frames = 0;
        ticks = 0;
        sinceStatsReset.restart();
    }
}

void Stats::draw(sf::RenderTarget &window, sf::Vector2f position) {
    frames++;
    setText(
            "FPS: " + std::to_string(fps) +
            "\nTPS: " + std::to_string(tps) +
            "\nF: " + std::to_string(frames) +
            "\nT: " + std::to_string(ticks) +
            (testSound ?
            "\nSound: " + fmt::format("{:%H:%M:%S}", std::chrono::duration<float>(testSound->getPlayingOffset().asSeconds()))
            : "")
            );
    if(sinceStatsReset.getElapsedTime() >= sf::seconds(1)) {
        sinceStatsReset.restart();
        fps = frames;
        tps = ticks;
        frames = 0;
        ticks = 0;
    }
    UILabel::draw(window, position);
}

bool Stats::isAlwaysVisible() const {
    return true;
}
