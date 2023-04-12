#ifndef OOP_TESTSCENE_H
#define OOP_TESTSCENE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Scene.h"
#include <atomic>
#include <mutex>

class TestScene : public Scene {
    sf::Text timeText;
    sf::Text statsText;
    sf::Font font;
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
    void updateStats();
    void updateTime();
public:
    TestScene();
    void draw(sf::RenderWindow &window) override;
    bool handleEvent(sf::Event &event) override;
    TickResult tick() override;
};

#endif //OOP_TESTSCENE_H
