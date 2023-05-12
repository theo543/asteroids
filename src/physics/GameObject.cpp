#include "GameObject.h"

void GameObject::setVelocity(sf::Vector2f velocity_) {
    this->velocity = velocity_;
}

void GameObject::setAngularVelocity(float angularVelocity_) {
    this->angularVelocity = angularVelocity_;
}

sf::Vector2f GameObject::getVelocity() const {
    return velocity;
}

float GameObject::getAngularVelocity() const {
    return angularVelocity;
}

void GameObject::accelerate(sf::Vector2f acceleration) {
    velocity += acceleration;
}

void GameObject::accelerateAngular(float acceleration) {
    angularVelocity += acceleration;
}

void GameObject::markForRemoval() {
    toBeRemoved = true;
}

GameObject::GameObject() : boundingRadius(0), toBeRemoved(false), velocity(0, 0), angularVelocity(0) {}

GameObject::GameObject(const GameObject &other) : transform(other.transform), boundingRadius(other.boundingRadius),
                                                  velocity(other.velocity), angularVelocity(other.angularVelocity) {
    toBeRemoved = false;
}

sf::Vector2f GameObject::getPosition() const {
    return transform.getPosition();
}

void GameObject::setPosition(sf::Vector2f position) {
    transform.setPosition(position);
}

float GameObject::getBoundingRadius() {
    return boundingRadius;
}
