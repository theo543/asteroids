#include <cmath>
#include "bounce_test/Player.h"
#include "bounce_test/Bouncy.h"
#include "physics/Physics.h"

void Player::draw(sf::RenderTarget &window, const Physics&) {
    window.draw(shape, pData.getTransform());
    window.draw(direction, pData.getTransform());
}

const float Player::acceleration = 1000;

void Player::tick(Physics &physics) {
    float mul = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mul += 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mul += -1;
    auto rot = sf::Transformable{};
    rot.setRotation(pData.getRotation());
    pData.accelerate(mul * rot.getTransform().transformPoint(sf::Vector2f{acceleration, acceleration}) * physics.getTickLen().asSeconds());
    mul = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mul += -1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mul += 1;
    pData.setAngularVelocity(mul * 360);
}

std::unique_ptr<GameObject> Player::clone() {
    return std::make_unique<Player>(*this);
}

void Player::collide(GameObject &other, Physics&) {
    if(&other == this) return;
    other.markForRemoval();
}

Player::Player(sf::Vector2f size, sf::Vector2f position, float rotation) : shape(size) {
    shape.setOrigin(size / 2.f);
    pData.initialize_collision(shape);
    pData.setRotation(rotation);
    pData.setPosition(position);
    direction.setRadius(10);
    direction.setFillColor(sf::Color::Blue);
    direction.setOrigin(2.1f * sf::Vector2f{10, 10} - shape.getSize() / 2.f);
}

Player::Player(const Player &other) = default;

