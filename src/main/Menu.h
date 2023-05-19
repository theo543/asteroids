#ifndef OOP_MENU_H
#define OOP_MENU_H

#include "world/WorldBase.h"

class Menu : public WorldBase {
protected:
    void onLoadWorld(sf::RenderWindow &window) override;
    void drawWorld(sf::RenderWindow &window) override;
    SwitchCommand tickWorld() override;
public:
    Menu();
};


#endif //OOP_MENU_H
