#ifndef OOP_TIMERTEST_H
#define OOP_TIMERTEST_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "world/WorldBase.h"
#include "menu/Stats.h"
#include <atomic>
#include <mutex>

class TimerTest : public WorldBase {
    sf::Font font;
    sf::SoundBuffer buffer;
    std::shared_ptr<sf::Sound> testSound;
    sf::Clock exitTimer;
    sf::Time displayTime;
    const static sf::Time exitDelay;
    bool areYouSure = false;
    std::shared_ptr<UILabel> timeText;
    void updateTime();
protected:
    void onLoadWorld(sf::RenderWindow &window) override;
    void drawWorld(sf::RenderWindow &window) override;
    SwitchCommand tickWorld() override;
public:
    TimerTest();
    ~TimerTest() override;
};

#endif //OOP_TIMERTEST_H
