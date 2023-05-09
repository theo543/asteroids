#include "bounce_test/Bouncy.h"
#include "physics/Physics.h"

Bouncy::Bouncy(sf::Vector2f position, sf::Vector2f velocity_, float radius) : GameObject() {
    boundingRadius = radius;
    velocity = velocity_;
    transform.setPosition(position);
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setFillColor(sf::Color::Red);
}

Bouncy::Bouncy(const Bouncy &other) = default;

void Bouncy::draw(sf::RenderWindow &window, const Physics &physics) {
    if(colliding) {
        const sf::Color colColor = sf::Color::Red;
        shape.setOutlineColor(colColor);
        shape.setFillColor(colColor);
    } else {
        int posX, posY;
        auto position = transform.getPosition();
        auto worldSize = physics.getWorldBorder();
        posX = static_cast<int>(position.x / worldSize.x * 255.0f);
        posY = static_cast<int>(position.y / worldSize.y * 255.0f);
        shape.setFillColor(sf::Color(posX, posY, 0));
        posX = static_cast<int>(velocity.x);
        posY = static_cast<int>(velocity.y);
        shape.setOutlineColor(sf::Color(posX, posY, 0));
    }
    shape.setOutlineThickness(2.0f);
    window.draw(shape, transform.getTransform());
}

void Bouncy::tick(Physics&) {
    colliding = false;
}

std::unique_ptr<GameObject> Bouncy::clone() {
    return std::make_unique<Bouncy>(*this);
}

void Bouncy::collide(GameObject&, Physics&) {
    colliding = true;
}

float Bouncy::getRadius() { return shape.getRadius(); }
