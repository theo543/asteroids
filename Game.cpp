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

void Game::renderingThread() {
    window.setActive(true);
    resetTime(); // Don't count time spent getting to this point
    while(window.isOpen()) {
        if(shouldExit) {
            window.close();
            break;
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

void Game::eventPollingThread() {
    sf::Event event{};
    while(window.isOpen()) {
        bool success = window.waitEvent(event);
        if(!success)
            continue;
        if (!scene->handleEvent(event))
            continue;
        switch (event.type) {
            case sf::Event::Closed:
                shouldExit = true;
                return;
            case sf::Event::Resized:
            default:
                break;
        }
    }
}

void Game::mainLoop() {
    window.setActive(false);
    sf::Thread renderThread(&Game::renderingThread, this);
    renderThread.launch();
    eventPollingThread();
    renderThread.wait(); // Join
}
