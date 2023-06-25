#include "SFML.h"
#include "physics/CollisionTest.h"
#include "world/SwitchFactory.h"
#include "resources/GlobalLoaders.h"

CollisionTest::CollisionTest() : circle(75), rect({75, 200}), asteroid(75), circleData(), rectData(), asteroidData(),
label(std::make_shared<UILabel>("", GlobalLoaders::Fonts().load("PublicPixelTTF"), 24, sf::Color::White, sf::Color::White)) {
    ui.setHideBehavior(UI::HideBehavior::Exit);
    ui.addItem(label);
    label->setFixedPosition(sf::Vector2f(0, 0));
    circle.setFillColor(sf::Color::Red);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circleData.initialize_collision(circle);
    circleData.setPosition({200, 200});
    rect.setFillColor(sf::Color::Blue);
    rect.setOrigin(rect.getSize() / 2.f);
    rectData.initialize_collision(rect);
    rectData.setPosition({300, 400});
    asteroid.setFillColor(sf::Color::Transparent);
    asteroid.setOutlineColor(sf::Color::White);
    asteroid.setOutlineThickness(2.0f);
    asteroid.setOrigin(asteroid.getRadius(), asteroid.getRadius());
    asteroidData.initialize_collision(asteroid);
    asteroidData.setPosition({600, 300});
}

void CollisionTest::drawWorld(sf::RenderWindow &window) {
    auto mousePos = sf::Mouse::getPosition(window);
    auto mouseVec = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    auto circleInts = circleData.point_intersections(mouseVec);
    auto rectInts = rectData.point_intersections(mouseVec);
    std::string text;
    text += "Circle intersections: " + std::to_string(circleInts) + "\n";
    text += "Rect intersections: " + std::to_string(rectInts) + "\n";
    text += "Asteroid intersections: " + std::to_string(asteroidData.point_intersections(mouseVec)) + "\n";
    text += "Mouse pos: " + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + "\n";
    text += "Circle pos: " + std::to_string(circleData.getPosition().x) + ", " + std::to_string(circleData.getPosition().y) + "\n";
    text += "Rect pos: " + std::to_string(rectData.getPosition().x) + ", " + std::to_string(rectData.getPosition().y) + "\n";
    text += "Asteroid pos: " + std::to_string(asteroidData.getPosition().x) + ", " + std::to_string(asteroidData.getPosition().y) + "\n";
    label->setText(text);
    window.draw(circle, circleData.getTransform());
    window.draw(rect, rectData.getTransform());
    window.draw(asteroid, asteroidData.getTransform());
}

SwitchCommand CollisionTest::tickWorld() {
    return SwitchFactory::empty();
}
