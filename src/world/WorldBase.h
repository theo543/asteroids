#ifndef OOP_WORLDBASE_H
#define OOP_WORLDBASE_H

#include "main/WorldInterface.h"
#include "menu/UI.h"
#include <functional>

/**
 * Base class for using the UI. Abstracts away the polling thread.
 */
class WorldBase : public WorldInterface {
protected:
    UI ui;
    virtual void initWorld(sf::RenderWindow &window) = 0;
    virtual void drawWorld(sf::RenderWindow &window) = 0;
    virtual TickResult tickWorld() = 0;
public:
    WorldBase(bool enableDebugLagKey = false);
    void init(sf::RenderWindow &window) final;
    void draw(sf::RenderWindow &window) final;
    void handleEvent(sf::Event &event) final;
    TickResult tick() final;
};


#endif //OOP_WORLDBASE_H
