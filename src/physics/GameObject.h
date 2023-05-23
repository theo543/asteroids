#ifndef OOP_GAMEOBJECT_H
#define OOP_GAMEOBJECT_H

#include <memory>
#include "SFML.h"

class Physics;

class GameObject {
protected:
    /// The position is of the <i>center</i> of the object.
    sf::Transformable transform;
    float boundingRadius;
    bool toBeRemoved;
    sf::Vector2f velocity;
    float angularVelocity;
    GameObject();
    GameObject(const GameObject &other);
public:
    virtual void draw(sf::RenderTarget &window, const Physics &physics) = 0;
    virtual void tick(Physics &physics) = 0;
    virtual std::unique_ptr<GameObject> clone() = 0;
    virtual void collide(GameObject &other, Physics &physics) = 0;
    virtual ~GameObject() = default;
    void markForRemoval();
    float getBoundingRadius();
    void setVelocity(sf::Vector2f velocity);
    void setAngularVelocity(float angularVelocity);
    sf::Vector2f getVelocity() const;
    float getAngularVelocity() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);
    void accelerate(sf::Vector2f acceleration);
    void accelerateAngular(float acceleration);
    friend class Physics;
};


#endif //OOP_GAMEOBJECT_H
