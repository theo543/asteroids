#ifndef OOP_PHYSICS_H
#define OOP_PHYSICS_H

#include "physics/GameObject.h"
#include <vector>
#include <memory>
#include <functional>

class Physics {
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    sf::Time tickLen;
    bool collisions;
    sf::Vector2f worldBorder;
public:
    explicit Physics(sf::Time tickLen);
    [[nodiscard]] sf::Time getTickLen() const;
    void setWorldBorder(sf::Vector2f border);
    [[nodiscard]] sf::Vector2f getWorldBorder() const;
    void setCollisionsEnabled(bool enable);
    void addGameObject(std::unique_ptr<GameObject> gameObject);
    bool forEachGameObject(const std::function<void(GameObject &, Physics &)> &func);
    void tick();
    void draw(sf::RenderWindow &window);
};


#endif //OOP_PHYSICS_H
