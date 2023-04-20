#ifndef OOP_BOUNCETEST_H
#define OOP_BOUNCETEST_H

#include "main/WorldInterface.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <atomic>
#include <random>

class BounceTest : public WorldInterface {
private:
    struct GameObject {
        sf::Vector2f position, velocity;
        sf::CircleShape shape;
    };
    std::vector<GameObject> objects;
    sf::Vector2f worldSize;
    sf::Time newObject;
    sf::Time bump;
    std::atomic_bool exit = false;
    std::mt19937 rng;
    float gen_nr(unsigned int max, bool absolute = true);
    sf::Vector2f gen_v(unsigned int max);
public:
    BounceTest();
    void init(sf::RenderWindow &window) override;
    void draw(sf::RenderWindow &window) override;
    void handleEvent(sf::Event &event) override;
    TickResult tick() override;
};


#endif //OOP_BOUNCETEST_H