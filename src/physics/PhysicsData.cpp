#include <cmath>
#include <cassert>
#include <numeric>
#include "utility/minmax.h"
#include "PhysicsData.h"

PhysicsData::PhysicsData() : mesh(), velocity(0.f, 0.f), angularVelocity(0), boundingRadius(0), base_aabb() {}

void PhysicsData::initialize(std::vector<sf::Vector2f> &&mesh_) {
    if(!mesh.empty()) throw std::runtime_error("Already initialized.");
    mesh = std::move(mesh_);
    for(const auto &point : mesh) {
        float w = point.x, h = point.y;
        float distance = std::sqrt(std::pow(w, 2.f) + std::pow(h, 2.f));
        ref_max(boundingRadius, distance);
        assert(distance >= 0);
    }
    assert(boundingRadius >= 0);
    base_aabb = AABB{mesh};
}

void PhysicsData::initialize(const sf::Shape &shape) {
    std::vector<sf::Vector2f> mesh_;
    std::size_t points = shape.getPointCount();
    for(std::size_t x = 0;x<points;x++) {
        mesh_.push_back(shape.getTransform().transformPoint(shape.getPoint(x)));
    }
    initialize(std::move(mesh_));
}


bool PhysicsData::isInitialized() const {
    return !mesh.empty();
}

void PhysicsData::setVelocity(sf::Vector2f velocity_) {
    velocity = velocity_;
}

void PhysicsData::setAngularVelocity(float angularVelocity_) {
    angularVelocity = angularVelocity_;
}

sf::Vector2f PhysicsData::getVelocity() const {
    return velocity;
}

float PhysicsData::getAngularVelocity() const {
    return angularVelocity;
}

float PhysicsData::getBoundingRadius() const {
    return boundingRadius;
}

void PhysicsData::accelerate(sf::Vector2f acceleration) {
    velocity += acceleration;
}

void PhysicsData::accelerateAngular(float acceleration) {
    angularVelocity += acceleration;
}

bool PhysicsData::aabb_collides(const PhysicsData &other) {
    return base_aabb.transform(getTransform()).collides(other.base_aabb.transform(other.getTransform()));
}
