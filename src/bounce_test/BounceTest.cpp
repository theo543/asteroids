#include "bounce_test/BounceTest.h"
#include "basic_test/Stats.h"
#include "../embedded_fwd.h"
#include <cassert>

BounceTest::BounceTest() : WorldBase(false), rng(std::random_device()()) {
    setTiming(sf::seconds(static_cast<float>(1.0L / 120.0L)), 24);
    f.loadFromMemory(PublicPixelTTF.data(), PublicPixelTTF.size());
    stats = std::make_shared<Stats>(f);
    ui.addItem(stats);
    ui.setHideBehavior(UI::HideBehavior::Exit);
}

void BounceTest::initWorld(sf::RenderWindow &window) {
    window.setTitle("Bouncy");
    worldSize = {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)};
}

void BounceTest::drawWorld(sf::RenderWindow &window) {
    for(auto &object : objects) {
        object.shape.setPosition(object.position);
        int posX, posY;
        posX = static_cast<int>(object.position.x / worldSize.x * 255.0f);
        posY = static_cast<int>(object.position.y / worldSize.y * 255.0f);
        object.shape.setFillColor(sf::Color(posX, posY, 0));
        posX = static_cast<int>(object.velocity.x);
        posY = static_cast<int>(object.velocity.y);
        object.shape.setOutlineColor(sf::Color(posX, posY, 0));
        object.shape.setOutlineThickness(2.0f);
        window.draw(object.shape);
    }
}

float BounceTest::gen_nr(unsigned int max, bool absolute) {
    return static_cast<float>(rng() % max) - (absolute ? 0 : static_cast<float>(max) / 2.0f);
}

sf::Vector2f BounceTest::gen_v(unsigned int max) {
    return {gen_nr(max, false), gen_nr(max, false)};
}

SwitchCommand BounceTest::tickWorld() {
    stats->tickOccurred();
    if(newObject > sf::seconds(0.05f)) {
        objects.emplace_back(GameObject{sf::Vector2{400.0f, 350.0f}, gen_v(200) + gen_v(200), sf::CircleShape(1 + gen_nr(5))});
        assert(objects.back().shape.getRadius() > 0);
        objects.back().shape.setFillColor(sf::Color::Blue);
        newObject = sf::Time::Zero;
    }
    if(bump > sf::seconds(2.0f)) {
        sf::Vector2f bump_vector = gen_v(500);
        for(auto &object : objects) {
            object.velocity = object.velocity * 0.65f + bump_vector;
        }
        bump = sf::Time::Zero;
    }
    for(auto &object : objects) {
        object.position += object.velocity * getTimePerTick().asSeconds();
        const float boost = 1.10f;
        if(object.position.x < 0) {
            object.position.x = 0;
            object.velocity.x = -object.velocity.x * boost;
        } else if(object.position.x + object.shape.getRadius() * 2 > worldSize.x) {
            object.position.x = worldSize.x - object.shape.getRadius() * 2;
            object.velocity.x = -object.velocity.x * boost;
        } else if(object.position.y < 0) {
            object.position.y = 0;
            object.velocity.y = -object.velocity.y * boost;
        } else if(object.position.y + object.shape.getRadius() * 2 > worldSize.y) {
            object.position.y = worldSize.y - object.shape.getRadius() * 2;
            object.velocity.y = -object.velocity.y * boost;
        }
    }
    newObject += getTimePerTick();
    bump += getTimePerTick();
    if(exit) return SwitchFactory::pop();
    return SwitchFactory::empty();
}
