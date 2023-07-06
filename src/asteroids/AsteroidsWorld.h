#ifndef ASTEROIDS_ASTEROIDSWORLD_H
#define ASTEROIDS_ASTEROIDSWORLD_H

#include "world/WorldBase.h"
#include "physics/Physics.h"
#include <random>

class AsteroidsWorld : public WorldBase {
    const static sf::Time tickLen;
    Physics physics;
    sf::Time newObject;
    std::mt19937 rng;
    void newAsteroid();
protected:
    void onLoadWorld(sf::RenderWindow &window) override;
    void drawWorld(sf::RenderWindow &window) override;
    SwitchCommand tickWorld() override;
public:
    AsteroidsWorld();
    explicit AsteroidsWorld(bool screensaverMode);
};


#endif //ASTEROIDS_ASTEROIDSWORLD_H
