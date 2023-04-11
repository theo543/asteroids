#ifndef OOP_GAME_H
#define OOP_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "Scene.h"

class Game : sf::NonCopyable {
    sf::RenderWindow window;
    std::unique_ptr<Scene> scene;
    sf::Clock realTime;
    sf::Time sceneTime;
    void resetTime();
public:
    explicit Game(std::unique_ptr<Scene> firstScene);
    void mainLoop();
};


#endif //OOP_GAME_H
