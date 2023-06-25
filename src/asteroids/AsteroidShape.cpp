#include "asteroids/AsteroidShape.h"
#include "utility/minmax.h"
#include <random>
#include <numbers>
#include <cmath>

AsteroidShape::AsteroidShape(float radius, float variance, std::size_t pointCount) : radius(radius) {
    points.resize(pointCount);
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-variance, variance);
    float min_radius = radius / 5.f;
    float max_radius = radius * 2.5f;
    float ray = radius;
    for(std::size_t x = 0;x<pointCount;x++) {
        ray += dist(rng);
        ref_max(ray, min_radius);
        ref_min(ray, max_radius);
        float angle = (static_cast<float>(std::numbers::pi) * 2.f / static_cast<float>(pointCount)) * static_cast<float>(x);
        points[x] = sf::Vector2f{std::cos(angle) * ray, std::sin(angle) * ray};
    }
    update();
}

AsteroidShape::AsteroidShape(float radius) : AsteroidShape(radius, radius / 2.f, 10) {}

float AsteroidShape::getRadius() const {
    return radius;
}

std::size_t AsteroidShape::getPointCount() const {
    return points.size();
}

sf::Vector2f AsteroidShape::getPoint(std::size_t index) const {
    return points[index];
}
