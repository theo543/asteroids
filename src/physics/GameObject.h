#ifndef OOP_GAMEOBJECT_H
#define OOP_GAMEOBJECT_H

#include <memory>
#include <optional>
#include "SFML.h"
#include "physics/PhysicsData.h"

class Physics;

class GameObject {
protected:
    /// The position is of the <i>center</i> of the object.
    PhysicsData pData;
    bool toBeRemoved;
    GameObject();
    GameObject(const GameObject &other);
public:
    virtual void draw(sf::RenderTarget &window, const Physics &physics) = 0;
    virtual void tick(Physics &physics) = 0;
    virtual std::unique_ptr<GameObject> clone() = 0;
    virtual void collide(GameObject &other, Physics &physics) = 0;
    virtual ~GameObject() = default;
    void markForRemoval();
    void setVelocity(sf::Vector2f velocity);
    void setAngularVelocity(float angularVelocity);
    [[nodiscard]] sf::Vector2f getVelocity() const;
    [[nodiscard]] float getAngularVelocity() const;
    [[nodiscard]] sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);
    void accelerate(sf::Vector2f acceleration);
    void accelerateAngular(float acceleration);
    [[nodiscard]] bool hasPhysicsData() const;
    [[nodiscard]] float getBoundingRadius() const;
    [[nodiscard]] const PhysicsData &getPhysicsData() const;
    friend class Physics;
};


#endif //OOP_GAMEOBJECT_H
