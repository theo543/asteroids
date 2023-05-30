#include "GameObject.h"

void GameObject::setVelocity(sf::Vector2f velocity_) {
    pData.setVelocity(velocity_);
}

void GameObject::setAngularVelocity(float angularVelocity_) {
    pData.setAngularVelocity(angularVelocity_);
}

sf::Vector2f GameObject::getVelocity() const {
    return pData.getVelocity();
}

float GameObject::getAngularVelocity() const {
    return pData.getAngularVelocity();
}

void GameObject::accelerate(sf::Vector2f acceleration) {
    pData.accelerate(acceleration);
}

void GameObject::accelerateAngular(float acceleration) {
    pData.accelerateAngular(acceleration);
}

void GameObject::markForRemoval() {
    toBeRemoved = true;
}

GameObject::GameObject() : pData(), toBeRemoved(false) {}

GameObject::GameObject(const GameObject &other) : pData(other.pData), toBeRemoved(false) {}

sf::Vector2f GameObject::getPosition() const {
    return pData.getPosition();
}

float GameObject::getBoundingRadius() const {
    return pData.getBoundingRadius();
}

void GameObject::setPosition(sf::Vector2f position) {
    pData.setPosition(position);
}

bool GameObject::hasPhysicsData() const {
    return pData.isInitialized();
}

const PhysicsData &GameObject::getPhysicsData() const {
    return pData;
}
