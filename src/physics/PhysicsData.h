#ifndef ASTEROIDS_PHYSICSDATA_H
#define ASTEROIDS_PHYSICSDATA_H

#include <vector>
#include <optional>
#include "SFML.h"
#include "physics/AABB.h"

/**
 * The GameObject will instantiate this and then use it to store position and velocity data.
 * As far as it cares this is just a struct.
 * There will also be private stuff only Physics cares about used to resolve collisions.
 */
class PhysicsData : public sf::Transformable {
    std::vector<sf::Vector2f> base_mesh;
    sf::Vector2f velocity;
    float angularVelocity;
    float boundingRadius;
    AABB base_aabb;
    mutable AABB cached_aabb;
    mutable std::vector<sf::Vector2f> cached_mesh;
    mutable sf::Transform cached_transform;
    void update_cache() const;
public:
    PhysicsData();
    /// It'd be incovenient if we did this in the constructor.
    void initialize_collision(std::vector<sf::Vector2f> &&mesh_);
    void initialize_collision(const sf::Shape &shape);
    bool collisionInitialized() const;
    void setVelocity(sf::Vector2f velocity);
    void setAngularVelocity(float angularVelocity);
    [[nodiscard]] sf::Vector2f getVelocity() const;
    [[nodiscard]] float getAngularVelocity() const;
    [[nodiscard]] float getBoundingRadius() const;
    void accelerate(sf::Vector2f acceleration);
    void accelerateAngular(float acceleration);
    [[nodiscard]] const AABB &getAABB() const;
    [[nodiscard]] const std::vector<sf::Vector2f> &getMesh() const;
    bool aabb_collides(const PhysicsData &other) const;
    std::size_t point_intersections(const sf::Vector2f &point) const;
    bool point_inside(const sf::Vector2f &point) const;
    std::optional<sf::Vector2f> collides(const PhysicsData &other) const;

    friend class Physics;
};


#endif //ASTEROIDS_PHYSICSDATA_H
