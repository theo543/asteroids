#include "bounce_test/Bouncy.h"
#include "physics/Physics.h"

Bouncy::Bouncy(sf::Vector2f position, sf::Vector2f velocity_, float radius) : GameObject() {
    radius += 2.0f; // got to include the outline
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(radius, radius);
    shape.setRadius(radius);
    pData.initialize_collision(shape);
    radius -= 2.0f;
    shape.setOrigin(radius, radius);
    shape.setRadius(radius);
    shape.setRadius(radius);
    pData.setVelocity(velocity_);
    pData.setPosition(position);
}

Bouncy::Bouncy(const Bouncy &other) : GameObject(other), shape(other.shape) {}

void Bouncy::draw(sf::RenderTarget &window, const Physics &physics) {
    if(colliding) {
        const sf::Color colColor = sf::Color::Red;
        shape.setOutlineColor(colColor);
        shape.setFillColor(colColor);
    } else {
        int posX, posY;
        auto position = pData.getPosition();
        auto worldSize = physics.getWorldBorder();
        posX = static_cast<int>(position.x / worldSize.x * 255.0f);
        posY = static_cast<int>(position.y / worldSize.y * 255.0f);
        shape.setFillColor(sf::Color(posX, posY, 0));
        posX = static_cast<int>(pData.getVelocity().x);
        posY = static_cast<int>(pData.getVelocity().y);
        shape.setOutlineColor(sf::Color(posX, posY, 0));
    }
    shape.setOutlineThickness(2.0f);
    window.draw(shape, pData.getTransform());
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
