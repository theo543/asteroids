#include <cmath>
#include <cassert>
#include <numeric>
#include "utility/minmax.h"
#include "PhysicsData.h"

PhysicsData::PhysicsData() : base_mesh(), velocity(0.f, 0.f), angularVelocity(0), boundingRadius(0), base_aabb(), cached_aabb(), cached_transform() {}

void PhysicsData::initialize_collision(std::vector<sf::Vector2f> &&mesh_) {
    if(!base_mesh.empty()) throw std::runtime_error("Already initialized.");
    base_mesh = std::move(mesh_);
    for(const auto &point : base_mesh) {
        float w = point.x, h = point.y;
        float distance = std::sqrt(std::pow(w, 2.f) + std::pow(h, 2.f));
        ref_max(boundingRadius, distance);
        assert(distance >= 0);
    }
    assert(boundingRadius >= 0);
    base_aabb = AABB{base_mesh};
    update_cache();
}

void PhysicsData::initialize_collision(const sf::Shape &shape) {
    std::vector<sf::Vector2f> mesh_;
    std::size_t points = shape.getPointCount();
    for(std::size_t x = 0;x<points;x++) {
        mesh_.push_back(shape.getTransform().transformPoint(shape.getPoint(x)));
    }
    initialize_collision(std::move(mesh_));
}


bool PhysicsData::collisionInitialized() const {
    return !base_mesh.empty();
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

bool PhysicsData::aabb_collides(const PhysicsData &other) const {
    return getAABB().collides(other.getAABB());
}
#include <iostream>
void PhysicsData::update_cache() const {
    cached_aabb = base_aabb.transform(getTransform());
    cached_transform = getTransform();
    cached_mesh.resize(base_mesh.size());
    std::transform(base_mesh.begin(), base_mesh.end(), cached_mesh.begin(), [this](const auto &point) {
        return getTransform().transformPoint(point);
    });
}

const AABB &PhysicsData::getAABB() const {
    if(cached_transform != getTransform()) update_cache();
    return cached_aabb;
}

const std::vector<sf::Vector2f> &PhysicsData::getMesh() const {
    if(cached_transform != getTransform()) update_cache();
    return cached_mesh;
}

namespace {

    typedef sf::Vector2f Pt;

    double cross(const Pt &a, const Pt &b) {
        return a.x * b.y - a.y * b.x;
    }

    double orientation(const Pt &a, const Pt &b, const Pt &c) {
        return cross(b - a, c - a);
    }

    bool intersect(const Pt &A, const Pt &B, Pt C, Pt D) {
        auto o1 = orientation(A, B, C);
        auto o2 = orientation(A, B, D);
        auto o3 = orientation(C, D, A);
        auto o4 = orientation(C, D, B);
        if (o1 == 0 && o2 == 0 && o3 == 0 && o4 == 0) {
            return std::min(A.x, B.x) <= std::max(C.x, D.x) && std::min(C.x, D.x) <= std::max(A.x, B.x) &&
                   std::min(A.y, B.y) <= std::max(C.y, D.y) && std::min(C.y, D.y) <= std::max(A.y, B.y);
        }
        return (o1 * o2 < 0) && (o3 * o4 < 0);
    }

}

std::size_t PhysicsData::point_intersections(const sf::Vector2f &point) const {
    std::size_t intersections = 0;
    Pt segStart = point;
    Pt segEnd = {getAABB().getMaxX() + .5f, point.y};
    for(std::size_t x = 0;x<getMesh().size();x++) {
        std::size_t otherSegStartIndex = (x == 0) ? getMesh().size() - 1 : x - 1;
        Pt otherSegStart = getMesh()[otherSegStartIndex], otherSegEnd = getMesh()[x];
        intersections += intersect(segStart, segEnd, otherSegStart, otherSegEnd);
    }
    return intersections;
}

bool PhysicsData::point_inside(const sf::Vector2f &point) const {
    return point_intersections(point) % 2 == 1;
}

std::optional<sf::Vector2f> PhysicsData::collides(const PhysicsData &other) const {
    for(const auto &p : getMesh()) {
        if(other.point_inside(p)) return p; // cppcheck-suppress useStlAlgorithm ; doesn't return an optional
    }
    return std::nullopt;
}
