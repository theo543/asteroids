#ifndef OOP_STATS_H
#define OOP_STATS_H

#include "menu/UILabel.h"

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
    void draw(sf::RenderTarget &window, sf::Vector2f position) override;
    void notifyTick() override;
    bool isAlwaysVisible() const override;
};


#endif //OOP_STATS_H
