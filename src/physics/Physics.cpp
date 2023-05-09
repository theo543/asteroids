#include "Physics.h"
#include <cmath>

Physics::Physics(sf::Time tickLen) : tickLen(tickLen), collisions(false) {};

sf::Time Physics::getTickLen() const {
    return tickLen;
}

void Physics::addGameObject(std::unique_ptr<GameObject> gameObject) {
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
                sf::Vector2f point1 = object->transform.getTransform().transformPoint({0, 0});
                sf::Vector2f point2 = other->transform.getTransform().transformPoint({0, 0});
                float distance = std::sqrt(std::pow(point1.x - point2.x, 2.f) + std::pow(point1.y - point2.y, 2.f));
                if (distance < object->boundingRadius + other->boundingRadius) {
                    colliding[x] = other.get();
                    colliding[y] = object.get();
                }
            }
        }
        for (std::size_t x = 0, size = gameObjects.size(); x < size; x++) {
            if (colliding[x] != nullptr) {
                colliding[x]->collide(*colliding[x], *this);
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
        gameObjects[x]->transform.move(gameObjects[x]->velocity * tickLen.asSeconds());
        gameObjects[x]->transform.rotate(gameObjects[x]->angularVelocity * tickLen.asSeconds());
    }
}

void Physics::draw(sf::RenderWindow &window) {
    for (auto &gameObject : gameObjects) {
        gameObject->draw(window);
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
