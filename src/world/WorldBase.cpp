#include <SFML/Graphics/RenderWindow.hpp>
#include "WorldBase.h"

WorldBase::WorldBase(bool enableDebugLagKey) : WorldInterface(enableDebugLagKey) {
    ui.setExitHandler(nullptr);
    ui.setForwardKey(sf::Keyboard::Key::Down);
    ui.setBackKey(sf::Keyboard::Key::Up);
    ui.setOpen(true);
    ui.setHideKey(sf::Keyboard::Key::Escape);
}

void WorldBase::onLoadWorld(sf::RenderWindow&) {}
void WorldBase::onUnloadWorld(sf::RenderWindow&, bool) {}

void WorldBase::onLoad(sf::RenderWindow &window) {
    onLoadWorld(window);
    auto titleOverride = std::getenv("APP_WINDOW");
    if(titleOverride)
        window.setTitle(sf::String(titleOverride));
}

void WorldBase::onUnload(sf::RenderWindow &window, bool permanent) {
    onUnloadWorld(window, permanent);
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
    ui.notifyTick();
    return tickWorld();
}
