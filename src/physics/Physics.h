#ifndef OOP_PHYSICS_H
#define OOP_PHYSICS_H

#include <vector>
#include <memory>
#include <functional>
#include <SFML/System/Time.hpp>
#include "physics/GameObject.h"

namespace sf { class RenderTarget; }

class Physics {
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    sf::Time tickLen;
    bool collisions;
    bool boundsVisible;
    sf::Vector2f worldBorder;
public:
    explicit Physics(sf::Time tickLen);
    [[nodiscard]] sf::Time getTickLen() const;
    void setWorldBorder(sf::Vector2f border);
    [[nodiscard]] sf::Vector2f getWorldBorder() const;

    [[maybe_unused]] [[nodiscard]] std::size_t getNrObjects() const;
    void setCollisionsEnabled(bool enable);
    void setBoundsVisible(bool visible);
    void addGameObject(std::unique_ptr<GameObject> gameObject);
    bool forEachGameObject(const std::function<void(GameObject &, Physics &)> &func);
    void tick();
    void draw(sf::RenderTarget &window);
};


#endif //OOP_PHYSICS_H
