#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include "physics/GameObject.h"
#include "asteroids/AsteroidShape.h"

class Asteroid : public GameObject {
    AsteroidShape shape;
public:
    Asteroid(sf::Vector2f position, sf::Vector2f velocity, float radius);
    Asteroid(const Asteroid &other);
    void draw(sf::RenderTarget &window, const Physics &physics) override;
    void tick(Physics &physics) override;
    std::unique_ptr<GameObject> clone() override;
    void collide(GameObject &other, Physics &physics) override;
    float getRadius();
};


#endif //ASTEROIDS_ASTEROID_H
