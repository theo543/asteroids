#ifndef OOP_MAINLOOP_H
#define OOP_MAINLOOP_H

#include <memory>
#include <atomic>
#include <stack>
#include "SFML.h"

class WorldInterface;

class MainLoop : sf::NonCopyable {
    const inline static sf::Time tickLagWarningThreshold = sf::seconds(1);
    sf::RenderWindow window;
    std::stack<std::unique_ptr<WorldInterface>> prev_stack;
    std::unique_ptr<WorldInterface> world;
    /// Real time. The time that the world has to keep up with.
    sf::Clock realTime;
    /// Total time passed in the world. Incremented by the tick length every time a tickWorld happens.
    sf::Time worldTime;
    /**
     * This flag tells eventPollingThread to return to mainLoop.
     * It's set by renderingThread, which must return after setting it.
     */
    std::atomic_bool returnToMainLoop;
    /**
     * When returning to mainLoop, if this is not nullptr, the world will be replaced with this one.
     * If it's nullptr, the game will exit.
     * renderingThread is responsible for setting this.
     */
    std::unique_ptr<WorldInterface> nextScene;
    bool saveScene;
    void resetTime();
    /**
     * Entry point for the rendering thread. Main loop starts this before calling eventPollingThread.
     * It's responsible for rendering the world and keeping ticks in sync with real time.
     * Event polling has to be in a separate thread because it blocks on some interactions like dragging the window.
     */
    void renderingThread();
    /**
     * Main loop calls this function after starting the rendering thread.
     * It's responsible for polling events and passing them to the world.
     */
    void eventPollingThread();
    void resetThreadVariables();
    sf::Time getTimeDifference() const;
    bool ticksNeeded() const;
public:
    [[maybe_unused]] explicit MainLoop(std::unique_ptr<WorldInterface> firstScene);
    void mainLoop();
};


#endif //OOP_MAINLOOP_H
