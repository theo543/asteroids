#include "bounce_test/BounceTest.h"
#include "basic_test/Stats.h"
#include "../embedded_fwd.h"
#include "Bouncy.h"
#include "Player.h"

const sf::Time BounceTest::tickLen = sf::seconds(static_cast<float>(1.0L / 120.0L));

BounceTest::BounceTest() : WorldBase(false), physics(tickLen), rng(std::random_device()()) {
    setTiming(tickLen, 24);
    f.loadFromMemory(PublicPixelTTF.data(), PublicPixelTTF.size());
    stats = std::make_shared<Stats>(f);
    ui.addItem(stats);
    ui.setHideBehavior(UI::HideBehavior::Exit);
    physics.setCollisionsEnabled(true);
    physics.addGameObject(std::make_unique<Player>(sf::Vector2f {100.f, 100.f}, sf::Vector2f {50.f, 50.f}, 0.f));
}

void BounceTest::initWorld(sf::RenderWindow &window) {
    window.setTitle("Bouncy");
    physics.setWorldBorder({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
}

void BounceTest::drawWorld(sf::RenderWindow &window) {
    physics.draw(window);
}

float BounceTest::gen_nr(unsigned int max, bool absolute) {
    return static_cast<float>(rng() % max) - (absolute ? 0 : static_cast<float>(max) / 2.0f);
}

sf::Vector2f BounceTest::gen_v(unsigned int max) {
    return {gen_nr(max, false), gen_nr(max, false)};
}

SwitchCommand BounceTest::tickWorld() {
    stats->tickOccurred();
    physics.tick();
    if(newObject > sf::seconds(0.05f)) {
        auto bouncy = std::make_unique<Bouncy>(physics.getWorldBorder() / 2.f, gen_v(200) + gen_v(200), 1 + gen_nr(5));
        physics.addGameObject(std::move(bouncy));
        newObject = sf::Time::Zero;
    }
    if(bump > sf::seconds(2.0f)) {
        sf::Vector2f bump_vector = gen_v(50);
        physics.forEachGameObject([bump_vector](GameObject &object, Physics&) {
            object.setVelocity(object.getVelocity() * 0.65f + bump_vector);
        });
        bump = sf::Time::Zero;
    }
    physics.forEachGameObject([](GameObject &object, Physics& p) {
        auto radius = object.getBoundingRadius();
        auto position = object.getPosition();
        auto velocity = object.getVelocity();
        const float boost = 1.10f;
        auto worldSize = p.getWorldBorder();
        if(position.x < 0) {
            position.x = 0;
            velocity.x = -velocity.x * boost;
        } else if(position.x + radius * 2 > worldSize.x) {
            position.x = worldSize.x - radius * 2;
            velocity.x = -velocity.x * boost;
        } else if(position.y < 0) {
            position.y = 0;
            velocity.y = -velocity.y * boost;
        } else if(position.y + radius * 2 > worldSize.y) {
            position.y = worldSize.y - radius * 2;
            velocity.y = -velocity.y * boost;
        }
        object.setPosition(position);
        object.setVelocity(velocity);
    });
    newObject += getTimePerTick();
    bump += getTimePerTick();
    if(exit) return SwitchFactory::pop();
    return SwitchFactory::empty();
}
