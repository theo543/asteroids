#include <cmath>
#include "physics/Physics.h"
#include "physics/GameObject.h"

Physics::Physics(sf::Time tickLen) : tickLen(tickLen), collisions(false), boundsVisible(false), worldBorder(0, 0) {}

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
            if(!gameObjects[x]->pData.collisionInitialized()) continue;
            for (std::size_t y = x + 1; y < gameObjects.size(); y++) {
                if(!gameObjects[y]->pData.collisionInitialized()) continue;
                auto &object = gameObjects[x];
                auto &other = gameObjects[y];
                if(!object->pData.aabb_collides(other->pData)) continue;
                // if point from object is inside other
                auto col = object->pData.collides(other->pData);
                // or point from other is inside object
                if(!col.has_value()) col = other->pData.collides(object->pData);
                // then collision
                // this misses edge cases, but you can't get those without first having point in polygon, so it's fine as long as ticks are short
                if(col.has_value()) {
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
            auto shape = gameObject->pData.getAABB().toRect();
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
