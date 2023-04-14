#include <iostream>
#include <thread>
#include "Game.h"
#include "Defer.h"

[[maybe_unused]] Game::Game(std::unique_ptr<Scene> firstScene) : window(), scene(std::move(firstScene)), realTime(), sceneTime(), returnToMainLoop(false), nextScene(nullptr) {
    // NOTE: sync with env variable APP_WINDOW from .GitHub/workflows/cmake.yml:30
    window.create(sf::VideoMode({800, 700}), "Current Time", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setActive(false);
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

sf::Time Game::getTimeDifference() const {
    return realTime.getElapsedTime() - sceneTime;
}

bool Game::sceneNeedsCatchup() const {
    return getTimeDifference() >= scene->getTimePerTick();
}

void Game::renderingThread() {
    window.setActive(true); // just in case SFML doesn't do this automatically
    DeferWrapper contextGuard([this] {window.setActive(false);}); // when this thread ends, release OpenGL context
    DeferWrapper returnFlagGuard([this] {returnToMainLoop = true;}); // when this thread ends, signal event loop to return
    resetTime(); // Don't count time spent getting to this point
    while(window.isOpen()) {
        if(scene->isLagSimulationDebugEnabled() && window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            sceneTime -= sf::seconds(60);
        window.clear(scene->getBackgroundColor());
        scene->draw(window);
        window.display();
        for(unsigned int ticksAllowed = scene->getMaxTicksPerFrame(); sceneNeedsCatchup() && ticksAllowed > 0; ticksAllowed--) {
            Scene::TickResult tr = scene->tick();
            sceneTime += scene->getTimePerTick();
            if(tr.has_value()) {
                nextScene = std::move(tr.value());
                return;
            }
        }
        if(sceneNeedsCatchup() && getTimeDifference() >= tickLagWarningThreshold)
            sf::err()<<"WARNING: The game is lagging behind real time by "<<getTimeDifference().asSeconds()<<" seconds."<<std::endl;
    }
    sf::err()<<"WARNING: The window has unexpectedly closed. Errors may occur."<<std::endl;
}

void Game::eventPollingThread() {
    sf::Event event{};
    while(window.isOpen() && !returnToMainLoop) {
        while(window.pollEvent(event))
            scene->handleEvent(event);
        // for some reason, the window closes twice as fast if I sleep here instead of waitEvent
        // SFML does the same thing in waitEvent, so I don't know why it's different here
        sf::sleep(sf::milliseconds(10));
    }
}

void Game::resetThreadVariables() {
    returnToMainLoop = false;
    nextScene = nullptr;
}

void Game::mainLoop() {
    while(scene != nullptr) {
        resetThreadVariables();
        window.setActive(false); // this should never be necessary, but just in case some SFML function automatically activates the context
        std::thread renderThread(&Game::renderingThread, this); // start rendering thread - this is needed because polling events blocks when a title bar button is pressed or the window is dragged
        eventPollingThread(); // no need to start a new thread, just call the function (on some platforms you can't even poll events from secondary threads anyway)
        renderThread.join();
        scene = std::move(nextScene); // if nextScene is nullptr (i.e. render thread didn't exit because of a scene change), the loop will end
    }
    window.close();
}
