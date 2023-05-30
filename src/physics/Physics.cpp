#include <cmath>
#include "physics/Physics.h"
#include "physics/GameObject.h"

Physics::Physics(sf::Time tickLen) : tickLen(tickLen), collisions(false), boundsVisible(false), worldBorder(0, 0) {}

sf::Time Physics::getTickLen() const {
    return tickLen;
}

void Physics::addGameObject(std::unique_ptr<GameObject> gameObject) {
    if(!gameObject->hasPhysicsData()) throw std::invalid_argument("Object has no physics data"); // TODO put a custom exception here
    gameObjects.push_back(std::move(gameObject));
}

void Physics::tick() {
    for (auto &gameObject : gameObjects) {
        gameObject->tick(*this);
    }
    if(collisions) {
        std::vector<GameObject*> colliding(gameObjects.size(), nullptr);
        for (std::size_t x = 0; x < gameObjects.size(); x++) {
            for (std::size_t y = x + 1; y < gameObjects.size(); y++) {
                auto &object = gameObjects[x];
                auto &other = gameObjects[y];
                if (object.get() == other.get()) continue;
                if (object->pData.aabb_collides(other->pData)) {
                    /// TODO actual narrow phase collision detection
                    colliding[x] = other.get();
                    colliding[y] = object.get();
                }
            }
        }
        for (std::size_t x = 0, size = gameObjects.size(); x < size; x++) {
            if (colliding[x] != nullptr) {
                gameObjects[x]->collide(*colliding[x], *this);
            }
        }
    }
    for(std::size_t x = 0, size = gameObjects.size();x < size;x++) {
        if(gameObjects[x]->toBeRemoved) {
            swap(gameObjects[x], gameObjects.back());
            gameObjects.pop_back();
            x--;
            size--;
            continue;
        }
        gameObjects[x]->pData.move(gameObjects[x]->pData.velocity * tickLen.asSeconds());
        gameObjects[x]->pData.rotate(gameObjects[x]->pData.angularVelocity * tickLen.asSeconds());
    }
}

void Physics::draw(sf::RenderTarget &window) {
    for (auto &gameObject : gameObjects) {
        gameObject->draw(window, *this);
        if(boundsVisible) {
            auto shape = gameObject->pData.base_aabb.transform(gameObject->pData.getTransform()).toRect();
            shape.setFillColor(sf::Color(0, 0, 255, 75));
            shape.setOutlineColor(sf::Color::Red);
            shape.setOutlineThickness(1.0f);
            window.draw(shape);
        }
    }
}

bool Physics::forEachGameObject(const std::function<void(GameObject &, Physics &)> &func) {
    std::size_t size = gameObjects.size();
    for(std::size_t x = 0;x < size;x++) {
        func(*gameObjects[x], *this);
    }
    return size != gameObjects.size();
}

void Physics::setCollisionsEnabled(bool enable) {
    collisions = enable;
}

void Physics::setBoundsVisible(bool visible) {
    boundsVisible = visible;
}

sf::Vector2f Physics::getWorldBorder() const {
    return worldBorder;
}

void Physics::setWorldBorder(sf::Vector2f border) {
    worldBorder = border;
}

[[maybe_unused]] std::size_t Physics::getNrObjects() const {
    return gameObjects.size();
}
