#ifndef OOP_WORLDBASE_H
#define OOP_WORLDBASE_H

#include "world/WorldInterface.h"
#include "menu/UI.h"
#include <functional>

/**
 * Base class for using the UI. Abstracts away the polling thread.
 */
class WorldBase : public WorldInterface {
protected:
    UI ui;
    virtual void onLoadWorld(sf::RenderWindow &window);
    virtual void onUnloadWorld(sf::RenderWindow &window, bool permanent);
    virtual void drawWorld(sf::RenderWindow &window) = 0;
    virtual SwitchCommand tickWorld() = 0;
public:
    explicit WorldBase(bool enableDebugLagKey = false);
    void onLoad(sf::RenderWindow &window) final;
    void onUnload(sf::RenderWindow &window, bool permanent) final;
    void draw(sf::RenderWindow &window) final;
    void handleEvent(sf::Event &event) final;
    SwitchCommand tick() final;
};


#endif //OOP_WORLDBASE_H
