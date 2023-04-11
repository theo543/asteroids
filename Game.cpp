#include <iostream>
#include "Game.h"

Game::Game(std::unique_ptr<Scene> firstScene) : window(), scene(std::move(firstScene)), realTime(), sceneTime() {
    // NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:30
    window.create(sf::VideoMode({800, 700}), "Current Time", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    auto desktop = sf::VideoMode::getDesktopMode();
    // This is safe because screen size isn't nearly as big as 32-bit int. If it is, you have other problems.
    // We also account for the window title bar
    window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2 - 50)); //NOLINT
}

void Game::resetTime() {
    realTime.restart();
    sceneTime = realTime.getElapsedTime();
}

void Game::mainLoop() {
    resetTime(); // Don't want init overhead to count towards the first tick
    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) { //TODO: Event handling MUST be in another thread eventually to not freeze the game
            if(!scene->handleEvent(event))
                continue;
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                default:
                    break;
            }
        }
        window.clear(scene->getBackgroundColor());
        scene->draw(window);
        window.display();
        unsigned int ticksAllowed = scene->getMaxTicksPerFrame();
        while((realTime.getElapsedTime() - sceneTime) >= scene->getTimePerTick()) {
            sceneTime += scene->getTimePerTick();
            ticksAllowed--;
            if(ticksAllowed == 0) {
                sf::err() << "WARNING: Game time falling behind real time. Skipping ticks." << std::endl;
                sceneTime = realTime.getElapsedTime();
                break;
            }
            Scene::TickResult tr = scene->tick();
            if(!tr.has_value())
                continue;
            if(!tr.value())
                window.close();
            else
                scene = std::move(tr.value());
        }
    }
}
