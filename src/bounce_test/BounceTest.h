#ifndef OOP_BOUNCETEST_H
#define OOP_BOUNCETEST_H

#include "basic_test/Stats.h"
#include "world/WorldBase.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <atomic>
#include <random>

class BounceTest : public WorldBase {
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
    sf::Font f;
    std::shared_ptr<Stats> stats;
protected:
    void initWorld(sf::RenderWindow &window) override;
    void drawWorld(sf::RenderWindow &window) override;
    TickResult tickWorld() override;
public:
    BounceTest();
};


#endif //OOP_BOUNCETEST_H
