#ifndef ASTEROIDS_COLLISIONTEST_H
#define ASTEROIDS_COLLISIONTEST_H

#include "world/WorldBase.h"
#include "physics/PhysicsData.h"
#include "menu/UILabel.h"
#include "asteroids/AsteroidShape.h"

class CollisionTest : public WorldBase {
private:
    sf::CircleShape circle;
    sf::RectangleShape rect;
    AsteroidShape asteroid;
    PhysicsData circleData;
    PhysicsData rectData;
    PhysicsData asteroidData;
    std::shared_ptr<UILabel> label;
protected:
    void drawWorld(sf::RenderWindow &window) override;
    SwitchCommand tickWorld() override;
public:
    CollisionTest();
};


#endif //ASTEROIDS_COLLISIONTEST_H
