#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include "physics/GameObject.h"

class Player : public GameObject {
    sf::RectangleShape shape;
    sf::CircleShape direction;
    static const float acceleration;
public:
    Player(sf::Vector2f size, sf::Vector2f position, float rotation);
    Player(const Player &other);
    void draw(sf::RenderWindow &window, const Physics &physics) override;
    void tick(Physics &physics) override;
    std::unique_ptr<GameObject> clone() override;
    void collide(GameObject &other, Physics &physics) override;
};


#endif //OOP_PLAYER_H
