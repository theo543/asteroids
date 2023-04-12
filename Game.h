#ifndef OOP_GAME_H
#define OOP_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <atomic>
#include "Scene.h"

class Game : sf::NonCopyable {
    sf::RenderWindow window;
    std::unique_ptr<Scene> scene;
    sf::Clock realTime;
    sf::Time sceneTime;
    std::atomic_bool shouldExit = false;
    void resetTime();
    /**
     * This is the rendering thread. Even though it's not the real main thread as far as the OS is concerned,
     * it's the main thread for the game, because the main thread freezes on some events (like window dragging).
     */
    void renderingThread();
    /**
     * This happens in the real main thread. It's responsible for polling events. Right now it needs atomic flags
     * to communicate with the rendering thread, but eventually there should be a lock-free queue for events and some sort of observer pattern.
     * The event thread should never do anything except set thread-safe flags or push events for the rendering thread to handle.
     */
    void eventPollingThread();
public:
    explicit Game(std::unique_ptr<Scene> firstScene);
    void mainLoop();
};


#endif //OOP_GAME_H
