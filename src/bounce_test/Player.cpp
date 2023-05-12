#include "bounce_test/Player.h"
#include "bounce_test/Bouncy.h"
#include "physics/Physics.h"
#include <cmath>

void Player::draw(sf::RenderWindow &window, const Physics&) {
    sf::CircleShape circle{boundingRadius};
    circle.setFillColor(sf::Color::Blue);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(0.0f);
    auto cT = transform;
    window.draw(circle, transform.getTransform());
}

void Player::tick(Physics &physics) {
    float mul = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mul += 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) mul += -1;
    auto rot = sf::Transformable{};
    rot.setRotation(transform.getRotation());
    velocity += mul * rot.getTransform().transformPoint(sf::Vector2f{1000, 1000}) * physics.getTickLen().asSeconds();
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
    transform.setOrigin(size.x / 2, size.y / 2);
    boundingRadius = std::sqrt(size.x * size.x + size.y * size.y) / 2.f;
}

Player::Player(const Player &other) = default;

