#include <algorithm>
#include "asteroids/AsteroidsWorld.h"
#include "asteroids/Asteroid.h"
#include "world/SwitchFactory.h"
#include "menu/UIOption.h"
#include "resources/GlobalLoaders.h"

const sf::Time AsteroidsWorld::tickLen = sf::seconds(1.f / 120.f);

AsteroidsWorld::AsteroidsWorld() : WorldBase(false), physics(tickLen), rng(std::random_device()()) {
    setTiming(tickLen, 24);
    physics.setCollisionsEnabled(true);
    physics.setBoundsVisible(false);
    auto Gray = sf::Color(128, 128, 128);
    auto pauseLabel = std::make_unique<UILabel>("Paused", GlobalLoaders::Fonts().load("PublicPixelTTF"), 24, sf::Color::White, sf::Color::White);
    pauseLabel->setMargin(0.f, 96.f);
    using FO = UIOption::Fill_Outline;
    auto exitOpt = std::make_unique<UIOption>("Exit", GlobalLoaders::Fonts().load("PublicPixelTTF"), FO{Gray, sf::Color::Transparent}, FO{sf::Color::Red, sf::Color::Black}, 24, []() {
        return SwitchFactory::pop();
    });
    ui.addItem(std::move(pauseLabel));
    ui.addItem(std::move(exitOpt));
    ui.setHideBehavior(UI::HideBehavior::Hide);
    ui.setOpen(false);
    ui.forAllSetPixelAlign(true);
}

void AsteroidsWorld::onLoadWorld(sf::RenderWindow &window) {
    window.setTitle("Asteroids");
    physics.setWorldBorder({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
}

void AsteroidsWorld::drawWorld(sf::RenderWindow &window) {
    physics.draw(window);
}

SwitchCommand AsteroidsWorld::tickWorld() {
    if(ui.isOpen()) return SwitchFactory::empty();
    if(newObject >= sf::seconds(0.50f)) {
        newAsteroid();
        newObject = sf::Time::Zero;
    } else {
        newObject += tickLen;
    }
    physics.tick();
    return SwitchFactory::empty();
}

void AsteroidsWorld::newAsteroid() {
    std::uniform_int_distribution<int> entryBorder(0, 3);
    using NFD = std::normal_distribution<float>;
    NFD XEntryPoint(physics.getWorldBorder().x / 2.f, physics.getWorldBorder().x / 3.f);
    NFD YEntryPoint(physics.getWorldBorder().y / 2.f, physics.getWorldBorder().y / 3.f);
    NFD angularVelocity(0.f, 25.f);
    NFD velocity(50.f, 25.f);
    NFD radius(20.f, 5.f);
    auto minVelocity = 20.f;
    auto minRadius = 10.f;

    auto pointOnBorder = [&](int choice) -> sf::Vector2f {
        switch(choice) {
            case 0: // top
                return {XEntryPoint(rng), 0.f};
            case 1: // right
                return {physics.getWorldBorder().x, YEntryPoint(rng)};
            case 2: // bottom
                return {XEntryPoint(rng), physics.getWorldBorder().y};
            default:
            case 3: // left
                return {0.f, YEntryPoint(rng)};
        }
    };

    float radiusVal = std::max(radius(rng), minRadius);
    auto entryPoint = pointOnBorder(entryBorder(rng));
    int goToChoice;
    while((goToChoice = entryBorder(rng)) == entryBorder(rng)); // make sure we don't go to the same side
    auto pointToGoTo = pointOnBorder(goToChoice);
    auto vectorToCenter = physics.getWorldBorder() / 2.f - pointToGoTo;
    auto vectorToGoTo = pointToGoTo - entryPoint;
    auto entryVelocity = vectorToGoTo + vectorToCenter * 0.5f; // pull towards center, so they can't just travel along the border
    entryVelocity = entryVelocity / std::sqrt(entryVelocity.x * entryVelocity.x + entryVelocity.y * entryVelocity.y); // normalize
    entryVelocity = entryVelocity * std::max(velocity(rng), minVelocity); // scale to velocity
    float entryAngularVelocity = angularVelocity(rng);

    auto asteroid = std::make_unique<Asteroid>(entryPoint, entryVelocity, radiusVal);
    asteroid->setAngularVelocity(entryAngularVelocity);
    auto maxHideSteps = 10;
    auto hideStep = asteroid->getVelocity() * -1.f;
    while(physics.isInBounds(*asteroid) && maxHideSteps-- > 0) {
        asteroid->move(asteroid->getVelocity() * -1.f);
        hideStep *= 1.5f;
    }
    physics.addGameObject(std::move(asteroid));
}
