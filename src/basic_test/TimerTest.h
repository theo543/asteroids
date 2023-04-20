#ifndef OOP_TIMERTEST_H
#define OOP_TIMERTEST_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "main/WorldInterface.h"
#include <atomic>
#include <mutex>

class TimerTest : public WorldInterface {
    sf::Text timeText;
    sf::Text statsText;
    sf::Font font;
    sf::SoundBuffer buffer;
    sf::Sound testSound;
    sf::Clock sinceStatsReset;
    sf::Clock exitTimer;
    sf::Time displayTime;
    const static sf::Time exitDelay;
    unsigned int frames = 0;
    unsigned int ticks = 0;
    unsigned int fps = 0;
    unsigned int tps = 0;
    std::mutex exitTimerMutex;
    std::atomic_bool areYouSure = false;
    std::atomic_bool exitConfirmed = false;
    void updateStats();
    void updateTime();
public:
    TimerTest();
    void init(sf::RenderWindow &window) override;
    void draw(sf::RenderWindow &window) override;
    void handleEvent(sf::Event &event) override;
    TickResult tick() override;
    ~TimerTest() override;
};

#endif //OOP_TIMERTEST_H
