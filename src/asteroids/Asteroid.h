#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include "physics/GameObject.h"
#include "asteroids/AsteroidShape.h"

/**
 * An asteroid.
 * Shape works like a SFML circle but the points move up and down - see AsteroidShape.
 * Marks itself for removal if it is outside the world border for too long.
 */
class Asteroid : public GameObject {
    AsteroidShape shape;
    sf::Time timeOutsideBorder;
    static const sf::Time defaultTimeOutsideBorder;
    bool colliding;
public:
    Asteroid(sf::Vector2f position, sf::Vector2f velocity, float radius);
    Asteroid(const Asteroid &other);
    void draw(sf::RenderTarget &window, const Physics &physics) override;
    void tick(Physics &physics) override;
    std::unique_ptr<GameObject> clone() override;
    void collide(GameObject &other, Physics &physics) override;
    [[nodiscard]] float getArea() const;
};


#endif //ASTEROIDS_ASTEROID_H
