#include "Asteroid.h"

Asteroid::Asteroid(sf::Vector2f position, sf::Vector2f velocity, float radius) : GameObject(), shape(radius) {
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.0f);
    shape.setOrigin(radius, radius);
    pData.initialize_collision(shape);
    pData.setVelocity(velocity);
    pData.setPosition(position);
}

Asteroid::Asteroid(const Asteroid&) = default;

void Asteroid::draw(sf::RenderTarget &window, const Physics&) {
    window.draw(shape, pData.getTransform());
}

void Asteroid::tick(Physics&) {
}

std::unique_ptr<GameObject> Asteroid::clone() {
    return std::make_unique<Asteroid>(*this);
}

void Asteroid::collide(GameObject &, Physics &) {

}

float Asteroid::getRadius() {
    return shape.getRadius();
}
