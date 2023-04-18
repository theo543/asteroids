#include <iostream>
#include <thread>
#include "main/MainLoop.h"
#include "utility/Defer.h"

[[maybe_unused]] MainLoop::MainLoop(std::unique_ptr<WorldInterface> firstScene) : window(), world(std::move(firstScene)), realTime(), worldTime(), returnToMainLoop(false), nextScene(nullptr) {
    // NOTE: sync with env variable APP_WINDOW from .GitHub/workflows/cmake.yml:30
    window.create(sf::VideoMode({800, 700}), "", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setActive(false);
    //window.setFramerateLimit(60);

    auto desktop = sf::VideoMode::getDesktopMode();
    // This is safe because screen size isn't nearly as big as 32-bit int. If it is, you have other problems.
    // We also account for the window title bar
    window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2 - 50)); //NOLINT
}

void MainLoop::resetTime() {
    realTime.restart();
    worldTime = realTime.getElapsedTime();
}

sf::Time MainLoop::getTimeDifference() const {
    return realTime.getElapsedTime() - worldTime;
}

bool MainLoop::ticksNeeded() const {
    return getTimeDifference() >= world->getTimePerTick();
}

void MainLoop::renderingThread() {
    window.setActive(true); // just in case SFML doesn't do this automatically
    DeferWrapper contextGuard([this] {window.setActive(false);}); // when this thread ends, release OpenGL context
    DeferWrapper returnFlagGuard([this] {returnToMainLoop = true;}); // when this thread ends, signal event loop to return
    resetTime(); // Don't count time spent getting to this point
    while(window.isOpen()) {
        if(world->isLagSimulationDebugEnabled() && window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            worldTime -= sf::seconds(60);
        window.clear(world->getBackgroundColor());
        world->draw(window);
        window.display();
        for(unsigned int ticksAllowed = world->getMaxTicksPerFrame(); ticksNeeded() && ticksAllowed > 0; ticksAllowed--) {
            WorldInterface::TickResult tr = world->tick();
            worldTime += world->getTimePerTick();
            if(tr.has_value()) {
                nextScene = std::move(tr.value());
                return;
            }
        }
        if(ticksNeeded() && getTimeDifference() >= tickLagWarningThreshold)
            sf::err()<<"WARNING: The game is lagging behind real time by "<<getTimeDifference().asSeconds()<<" seconds."<<std::endl;
    }
    sf::err()<<"WARNING: The window has unexpectedly closed. Errors may occur."<<std::endl;
}

void MainLoop::eventPollingThread() {
    sf::Event event{};
    while(window.isOpen() && !returnToMainLoop) {
        while(window.pollEvent(event))
            world->handleEvent(event);
        // for some reason, the window closes twice as fast if I sleep here instead of waitEvent
        // SFML does the same thing in waitEvent, so I don't know why it's different here
        sf::sleep(sf::milliseconds(10));
    }
}

void MainLoop::resetThreadVariables() {
    returnToMainLoop = false;
    nextScene = nullptr;
}

void MainLoop::mainLoop() {
    while(world != nullptr) {
        resetThreadVariables();
        window.setActive(false);
        world->init(window);
        window.setActive(false); // just in case init() changed it
        std::thread renderThread(&MainLoop::renderingThread, this); // start rendering thread - this is needed because polling events blocks when a title bar button is pressed or the window is dragged
        eventPollingThread(); // no need to start a new thread, just call the function (on some platforms you can't even poll events from secondary threads anyway)
        renderThread.join();
        world = std::move(nextScene); // if nextScene is nullptr (i.e. render thread didn't exit because of a world change), the loop will end
    }
    window.close();
}