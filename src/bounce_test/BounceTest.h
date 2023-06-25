#ifndef OOP_BOUNCETEST_H
#define OOP_BOUNCETEST_H

#include <vector>
#include <random>
#include "menu/Stats.h"
#include "world/WorldBase.h"
#include "physics/Physics.h"

class BounceTest : public WorldBase {
private:
    static const sf::Time tickLen;
    Physics physics;
    sf::Time newObject;
    sf::Time bump;
    std::mt19937 rng;
    float gen_nr(unsigned int max, bool absolute = true);
    sf::Vector2f gen_v(unsigned int max);
protected:
    void onLoadWorld(sf::RenderWindow &window) override;
    void drawWorld(sf::RenderWindow &window) override;
    SwitchCommand tickWorld() override;
public:
    BounceTest();
};


#endif //OOP_BOUNCETEST_H
