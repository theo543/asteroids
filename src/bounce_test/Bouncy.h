#ifndef OOP_BOUNCY_H
#define OOP_BOUNCY_H

#include "physics/GameObject.h"

class Bouncy : public GameObject {
    sf::CircleShape shape;
    static sf::Vector2f worldSize;
    bool colliding = false;
public:
    static void setWorldSize(sf::Vector2f size);
    Bouncy(sf::Vector2f position, sf::Vector2f velocity, float radius);
    Bouncy(const Bouncy &other);
    void draw(sf::RenderWindow &window) override;
    void tick(Physics &physics) override;
    std::unique_ptr<GameObject> clone() override;
    void collide(GameObject &other, Physics &physics) override;
    float getRadius();
};


#endif //OOP_BOUNCY_H
