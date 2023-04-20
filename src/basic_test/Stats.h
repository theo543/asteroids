#ifndef OOP_STATS_H
#define OOP_STATS_H

#include "menu/UILabel.h"
#include <SFML/Audio.hpp>

class Stats : public UILabel {
private:
    sf::Clock sinceStatsReset;
    unsigned int frames = 0;
    unsigned int ticks = 0;
    unsigned int fps = 0;
    unsigned int tps = 0;
    std::shared_ptr<sf::Sound> testSound;
public:
    explicit Stats(const sf::Font &font, std::shared_ptr<sf::Sound> testSound = nullptr);
    void draw(sf::RenderWindow &window, sf::Vector2f position) override;
    void tickOccurred();
};


#endif //OOP_STATS_H
