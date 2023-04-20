#include "WorldBase.h"

WorldBase::WorldBase(bool enableDebugLagKey) : WorldInterface(enableDebugLagKey) {
    ui.setExitHandler(nullptr);
    ui.setForwardKey(sf::Keyboard::Key::Down);
    ui.setBackKey(sf::Keyboard::Key::Up);
    ui.setOpen(true);
    ui.setHideKey(sf::Keyboard::Key::Escape);
}

void WorldBase::init(sf::RenderWindow &window) {
    initWorld(window);
}

void WorldBase::draw(sf::RenderWindow &window) {
    drawWorld(window);
    ui.update(window);
}

void WorldBase::handleEvent(sf::Event &event) {
    ui.pollingThreadHandleEvent(event);
}

SwitchCommand WorldBase::tick() {
    if(ui.hasTransition())
        return ui.getNextTransition();
    return tickWorld();
}
