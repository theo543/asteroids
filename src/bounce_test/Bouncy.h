#ifndef OOP_BOUNCY_H
#define OOP_BOUNCY_H

#include <SFML/Graphics/CircleShape.hpp>
#include "physics/GameObject.h"

class Bouncy : public GameObject {
    sf::CircleShape shape;
    bool colliding = false;
public:
    Bouncy(sf::Vector2f position, sf::Vector2f velocity, float radius);
    Bouncy(const Bouncy &other);
    void draw(sf::RenderTarget &window, const Physics &physics) override;
    void tick(Physics &physics) override;
    std::unique_ptr<GameObject> clone() override;
    void collide(GameObject &other, Physics &physics) override;
    float getRadius();
};


#endif //OOP_BOUNCY_H
