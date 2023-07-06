#include <thread>
#include "world/WorldInterface.h"
#include "main/MainLoop.h"
#include "utility/Defer.h"
#include "world/SwitchFactory.h"
#include "utility/message.h"

[[maybe_unused]] MainLoop::MainLoop(std::unique_ptr<WorldInterface> firstScene, bool fullscreen) : window(), fullscreen(fullscreen), world(std::move(firstScene)), realTime(), worldTime(), returnToMainLoop(false), requestRecreateWindow(false), nextScene(nullptr), saveScene(false) {
    createWindow();
    prev_stack.push(nullptr);
}

[[maybe_unused]] MainLoop::MainLoop(std::unique_ptr<WorldInterface> firstScene) : MainLoop(std::move(firstScene), false) {}

void MainLoop::createWindow() {
    if(fullscreen) {
        window.create(sf::VideoMode::getFullscreenModes()[0], "", sf::Style::Fullscreen, sf::ContextSettings(0, 0, 8));
    } else {
        window.create(sf::VideoMode({800, 700}), "", sf::Style::Titlebar | sf::Style::Close,
                      sf::ContextSettings(0, 0, 8));
    }
    window.setVerticalSyncEnabled(true);
    window.setActive(false);
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

void report_error_and_abort(const std::exception &e, const char *context) {
    std::string error_message = "An error occurred while running the game: ";
    error_message += e.what();
    error_message += "\n\nContext: ";
    error_message += context;
    error_message += "\n\nThe game will now exit.";
    error_box(error_message);
    std::abort();
}

void MainLoop::renderingThread() {
    try {
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
            SwitchCommand tr = world->tick();
            worldTime += world->getTimePerTick();
            if(tr.getFullscreen() != FullScreenCommand::NONE) {
                world->onUnload(window, false);
                window.setActive(false);
                if(tr.getFullscreen() == FullScreenCommand::ON)
                    fullscreen = true;
                else if(tr.getFullscreen() == FullScreenCommand::OFF)
                    fullscreen = false;
                else if (tr.getFullscreen() == FullScreenCommand::TOGGLE)
                    fullscreen = !fullscreen;
                requestRecreateWindow = true;
                requestRecreateWindow.wait(tr.getFullscreen());
                world->onLoad(window);
            }
            switch(tr.getAction()) {
                case SwitchAction::POP:
                    nextScene = std::move(prev_stack.top());
                    prev_stack.pop();
                    saveScene = false;
                    return;
                case SwitchAction::PUSH:
                    nextScene = tr.takeNextWorld();
                    saveScene = true;
                    return;
                case SwitchAction::REPLACE:
                    nextScene = tr.takeNextWorld();
                    saveScene = false;
                    return;
                case SwitchAction::CONTINUE:
                    break;
            }
        }
        if(ticksNeeded() && getTimeDifference() >= tickLagWarningThreshold)
            sf::err()<<"WARNING: The game is lagging behind real time by "<<getTimeDifference().asSeconds()<<" seconds."<<std::endl;
    }
    sf::err()<<"WARNING: The window has unexpectedly closed. Errors may occur."<<std::endl;
    } catch (const std::exception& e) {
        report_error_and_abort(e, "rendering thread");
    }
}

void MainLoop::eventPollingThread() {
    try {
        sf::Event event{};
        while (window.isOpen() && !returnToMainLoop) {
            if(requestRecreateWindow) {
                createWindow();
                requestRecreateWindow = false;
                requestRecreateWindow.notify_all();
            }
            while (window.pollEvent(event))
                world->handleEvent(event);
            // for some reason, the window closes twice as fast if I sleep here instead of waitEvent
            // SFML does the same thing in waitEvent, so I don't know why it's different here
            sf::sleep(sf::milliseconds(10));
        }
    } catch (const std::exception& e) {
        report_error_and_abort(e, "event polling thread");
    }
}

void MainLoop::resetThreadVariables() {
    returnToMainLoop = false;
    nextScene = nullptr;
    requestRecreateWindow = false;
}

void MainLoop::mainLoop() {
    while(world != nullptr) {
        resetThreadVariables();
        window.setActive(false);
        world->onLoad(window);
        window.setActive(false); // just in case onLoadWorld() changed it
        std::thread renderThread(&MainLoop::renderingThread, this); // start rendering thread - this is needed because polling events blocks when a title bar button is pressed or the window is dragged
        eventPollingThread(); // no need to start a new thread, just call the function (on some platforms you can't even poll events from secondary threads anyway)
        renderThread.join();
        world->onUnload(window, !saveScene);
        if(saveScene)
            prev_stack.push(std::move(world));
        world = std::move(nextScene); // if nextScene is nullptr (i.e. render thread didn't exit because of a world change), the loop will end
    }
    window.close();
}
