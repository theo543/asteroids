#ifndef OOP_GAME_H
#define OOP_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <atomic>
#include "scenes/Scene.h"

class Game : sf::NonCopyable {
    const inline static sf::Time tickLagWarningThreshold = sf::seconds(1);
    sf::RenderWindow window;
    /// This is the scene that's currently being rendered.
    std::unique_ptr<Scene> scene;
    /// Real time. The time that the scene has to keep up with.
    sf::Clock realTime;
    /// Total time passed in the scene. Incremented by the tick length every time a tick happens.
    sf::Time sceneTime;
    /**
     * This flag tells eventPollingThread to return to mainLoop.
     * It's set by renderingThread, which must return after setting it.
     */
    std::atomic_bool returnToMainLoop;
    /**
     * When returning to mainLoop, if this is not nullptr, the scene will be replaced with this one.
     * If it's nullptr, the game will exit.
     * renderingThread is responsible for setting this.
     */
    std::unique_ptr<Scene> nextScene;
    void resetTime();
    /**
     * Entry point for the rendering thread. Main loop starts this before calling eventPollingThread.
     * It's responsible for rendering the scene and keeping ticks in sync with real time.
     * Event polling has to be in a separate thread because it blocks on some interactions like dragging the window.
     */
    void renderingThread();
    /**
     * Main loop calls this function after starting the rendering thread.
     * It's responsible for polling events and passing them to the scene.
     * It cannot close the window or switch scenes, because the rendering thread is constantly using the scene and window.
     * So scenes must signal to the render thread using safe flags or queues.
     */
    void eventPollingThread();
    void resetThreadVariables();
    sf::Time getTimeDifference() const;
    bool sceneNeedsCatchup() const;
public:
    [[maybe_unused]] explicit Game(std::unique_ptr<Scene> firstScene);
    void mainLoop();
};


#endif //OOP_GAME_H
