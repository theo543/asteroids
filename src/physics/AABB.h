#ifndef ASTEROIDS_AABB_H
#define ASTEROIDS_AABB_H

#include "SFML.h"

class AABB {
    sf::Vector2f tl, br;
public:
    AABB();
    explicit AABB(const std::vector<sf::Vector2f> &mesh);
    [[nodiscard]] bool collides(const AABB &other) const;
    [[nodiscard]] AABB transform(sf::Transform t) const;
    [[nodiscard]] sf::RectangleShape toRect() const;
    [[nodiscard]] float getMaxX() const;
};

#endif //ASTEROIDS_AABB_H
