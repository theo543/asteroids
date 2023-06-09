#include "asteroids/AsteroidShape.h"
#include "utility/minmax.h"
#include <random>
#include <numbers>
#include <cmath>

AsteroidShape::AsteroidShape(float radius, float variance, std::size_t pointCount) : radius(radius) {
    points.resize(pointCount + 1);
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-variance, variance);
    float min_radius = radius / 5.f;
    float max_radius = radius * 2.5f;
    float ray = radius;
    sf::Vector2f centerPos = sf::Vector2f{radius, radius};
    for(std::size_t x = 0;x<pointCount;x++) {
        ray += dist(rng);
        ref_max(ray, min_radius);
        ref_min(ray, max_radius);
        float angle = (static_cast<float>(std::numbers::pi) * 2.f / static_cast<float>(pointCount)) * static_cast<float>(x);
        points[x] = centerPos + sf::Vector2f{std::cos(angle) * ray, std::sin(angle) * ray}; // all SFML shapes have top-left corner = (0, 0) by default
    }
    points[pointCount] = points[0]; // temporary for convenience in area calculation
    area = 0;
    // https://en.wikipedia.org/wiki/Shoelace_formula
    for(std::size_t x = 0;x<pointCount;x++) {
        area += points[x].x * points[x + 1].y - points[x + 1].x * points[x].y;
    }
    area /= 2.f;
    points.pop_back();
    update();
}

AsteroidShape::AsteroidShape(float radius) : AsteroidShape(radius, radius / 5.f, 20) {}

float AsteroidShape::getRadius() const {
    return radius;
}

std::size_t AsteroidShape::getPointCount() const {
    return points.size();
}

sf::Vector2f AsteroidShape::getPoint(std::size_t index) const {
    return points[index];
}

float AsteroidShape::getArea() const {
    return area;
}
