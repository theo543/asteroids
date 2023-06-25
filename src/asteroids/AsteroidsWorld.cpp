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
        using FD = std::uniform_real_distribution<float>;
        FD radius_dist(40.f, 60.f);
        FD border_dist(0, static_cast<float>(physics.getWorldBorder().x));
        FD angular_dist(10.f, 50.f);
        FD speed_dist(50.f, 100.f);
        auto asteroid = std::make_unique<Asteroid>(sf::Vector2f{border_dist(rng), 0},
                                                   sf::Vector2f{0, speed_dist(rng)},
                                                    radius_dist(rng));
        asteroid->setAngularVelocity(angular_dist(rng)); ///TODO origin doesn't work??? rotates around a corner
        physics.addGameObject(std::move(asteroid));
        newObject = sf::Time::Zero;
    } else {
        newObject += tickLen;
    }
    physics.tick();
    return SwitchFactory::empty();
}
