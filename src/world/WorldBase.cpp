#include "WorldBase.h"
#ifdef GITHUB_ACTIONS
#include <cassert>
#endif

WorldBase::WorldBase(bool enableDebugLagKey) : WorldInterface(enableDebugLagKey) {
    ui.setExitHandler(nullptr);
    ui.setForwardKey(sf::Keyboard::Key::Down);
    ui.setBackKey(sf::Keyboard::Key::Up);
    ui.setOpen(true);
    ui.setHideKey(sf::Keyboard::Key::Escape);
}

void WorldBase::onLoadWorld(sf::RenderWindow&) {}
void WorldBase::onUnloadWorld(sf::RenderWindow&, bool) {}

void WorldBase::onLoad(sf::RenderWindow &window, std::shared_ptr<Debouncer> debouncer_) {
    debouncer = debouncer_;
    onLoadWorld(window);
    auto titleOverride = std::getenv("APP_WINDOW");
#ifdef GITHUB_ACTIONS
    assert(titleOverride && std::string(titleOverride).size() != 0); // this should always be defined on CI so the saniziter scripts can find the window
#endif
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
    if(debouncer) {
        if (event.type == sf::Event::KeyPressed) {
            if (!debouncer->pressed(event.key.code))
                return;
        } else if (event.type == sf::Event::KeyReleased) {
            debouncer->released(event.key.code);
        }
    }
    ui.pollingThreadHandleEvent(event);
}

SwitchCommand WorldBase::tick() {
    if(ui.hasTransition())
        return ui.getNextTransition();
    ui.notifyTick();
    return tickWorld();
}
