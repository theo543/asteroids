#include "asteroids/Asteroid.h"
#include "physics/Physics.h"

const sf::Time Asteroid::defaultTimeOutsideBorder = sf::seconds(10);

Asteroid::Asteroid(sf::Vector2f position, sf::Vector2f velocity, float radius) : GameObject(), shape(radius), timeOutsideBorder(defaultTimeOutsideBorder) {
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

void Asteroid::tick(Physics &p) {
    if(p.isInBounds(*this)) {
        timeOutsideBorder = defaultTimeOutsideBorder;
        shape.setFillColor(sf::Color::Transparent);
    } else {
        shape.setFillColor(sf::Color::Red); // if this is ever visible, isInBounds is broken
        timeOutsideBorder -= p.getTickLen();
        if(timeOutsideBorder <= sf::Time::Zero) {
            markForRemoval();
        }
    }
}

std::unique_ptr<GameObject> Asteroid::clone() {
    return std::make_unique<Asteroid>(*this);
}

void Asteroid::collide(GameObject &, Physics &) {

}
