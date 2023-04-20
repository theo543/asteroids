#ifndef OOP_MENU_H
#define OOP_MENU_H

#include "world/WorldBase.h"

class Menu : public WorldBase {
    sf::Font f;
protected:
    void initWorld(sf::RenderWindow &window) override;
    void drawWorld(sf::RenderWindow &window) override;
    TickResult tickWorld() override;
};


#endif //OOP_MENU_H
