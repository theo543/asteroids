#include "WorldBase.h"

void WorldBase::init(sf::RenderWindow &window) {
    ui.setExitHandler([]() {
        return WorldInterface::EXIT();
    });
    ui.setForwardKey(sf::Keyboard::Key::Down);
    ui.setBackKey(sf::Keyboard::Key::Up);
    ui.setOpen(true);
    ui.setHideKey(sf::Keyboard::Key::Escape);
    initWorld(window);
}

void WorldBase::draw(sf::RenderWindow &window) {
    drawWorld(window);
    ui.update(window);
}

void WorldBase::handleEvent(sf::Event &event) {
    ui.pollingThreadHandleEvent(event);
}

WorldInterface::TickResult WorldBase::tick() {
    if(ui.hasTransition())
        return ui.getNextTransition();
    return tickWorld();
}
