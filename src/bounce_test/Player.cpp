#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>
#include "bounce_test/Player.h"
#include "bounce_test/Bouncy.h"
#include "physics/Physics.h"

void Player::draw(sf::RenderTarget &window, const Physics&) {
    window.draw(shape, transform.getTransform());
    window.draw(direction, transform.getTransform());
}

const float Player::acceleration = 1000;

void Player::tick(Physics &physics) {
    float mul = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mul += 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mul += -1;
    auto rot = sf::Transformable{};
    rot.setRotation(transform.getRotation());
    velocity += mul * rot.getTransform().transformPoint(sf::Vector2f{acceleration, acceleration}) * physics.getTickLen().asSeconds();
    mul = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mul += -1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mul += 1;
    angularVelocity = mul * 360;
}

std::unique_ptr<GameObject> Player::clone() {
    return std::make_unique<Player>(*this);
}

void Player::collide(GameObject &other, Physics&) {
    if(&other == this) return;
    other.markForRemoval();
}

Player::Player(sf::Vector2f size, sf::Vector2f position, float rotation) : shape(size) {
    transform.setRotation(rotation);
    transform.setPosition(position);
    shape.setOrigin(size / 2.f);
    boundingRadius = std::sqrt(size.x * size.x + size.y * size.y) / 2.f;
    direction.setRadius(10);
    direction.setFillColor(sf::Color::Blue);
    direction.setOrigin(2.1f * sf::Vector2f{10, 10} - shape.getSize() / 2.f);
}

Player::Player(const Player &other) = default;

