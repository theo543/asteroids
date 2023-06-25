#ifndef ASTEROIDS_ASTEROIDSHAPE_H
#define ASTEROIDS_ASTEROIDSHAPE_H

#include <vector>
#include "SFML.h"

class AsteroidShape : public sf::Shape {
    std::vector<sf::Vector2f> points;
    float radius;
public:
    AsteroidShape(float radius, float variance, std::size_t pointCount);
    explicit AsteroidShape(float radius);
    [[nodiscard]] float getRadius() const;
    [[nodiscard]] std::size_t getPointCount() const override;
    [[nodiscard]] sf::Vector2f getPoint(std::size_t index) const override;
};


#endif //ASTEROIDS_ASTEROIDSHAPE_H
