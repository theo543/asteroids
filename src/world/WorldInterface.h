#ifndef OOP_WORLDINTERFACE_H
#define OOP_WORLDINTERFACE_H

#include <optional>
#include <memory>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include "world/SwitchCommand.h"

namespace sf { class RenderWindow; class Event; }

/**
 * A WorldInterface is an interface between MainLoop and the game logic.
 * The main loop calls functions for drawing to the window, handling events, and ticking the game world.
 * Handling events is called from another thread to avoid blocking the main loop, so there must not be data races
 * between it and the drawing and ticking thread. To exit, signal to the tick function to return a non-empty SwitchCommand.
 * There are also some properties that can be set to indicate timing, background color, and debug options.
 */
class WorldInterface : sf::NonCopyable {
    /// Enables pressing 'T' to decrement game time by 60 seconds. Implemented in main loop, behind this flag.
    const bool enableLagSimulationDebug;
    /// The amount of in-game time that a tick represents. Independent of FPS, but maxTicksPerFrame must allow enough ticks per frame.
    sf::Time timePerTick = sf::seconds(1.f / 20.f);
    /// The maximum number of ticks that can be processed in a single frame. Should be set according to lowest supported FPS.
    unsigned int maxTicksPerFrame = 5;
    /// Color used for clearing the window before drawing.
    sf::Color backgroundColor = sf::Color::Black;
protected:
    explicit WorldInterface(bool enableLagSimulationDebug = false);
    /// Because maxTicksPerFrame should be set based on timePerTick and the lowest expected FPS, they are both set in the same function.
    void setTiming(sf::Time t_t, unsigned int max_t);
    void setBackgroundColor(sf::Color color);
public:
    /**
     * Called by the main loop before starting the rendering thread.
     * Allows things like changing the title of the window that can't be done from the constructor
     * because a world is constructed before being passed to the main loop.
     * If this world loads another using the PUSH action, onLoad will be called again after the new world exits.
     */
    virtual void onLoad(sf::RenderWindow &window) = 0;
    /**
     * Called by the main loop when stopping executing a WorldInterface (either because of a switch or exit).
     * Should allow pausing the main menu's soundtrack when entering the game and stuff like that.
      * @param permanent whether the world is about to be deleted after unload
     */
    virtual void onUnload(sf::RenderWindow &window, bool permanent) = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
    /**
     * <b>Will be called from event polling thread.</b>
     * It is called when an event is received from the window. Unfortunately, it can't mess with the world state since it's called from another thread.
     * Instead, it has to tell the other thread to do it. SFML provides real-time input so only the UI and close/resize events will involve this function.
     */
    virtual void handleEvent(sf::Event &event) = 0;
    /**
     * <b>Will be called from rendering thread.</b>
     * It should update the physics simulation.
     * @returns If it returns a non-empty optional, the world will be replaced with the one in the optional or exited if the optional is nullptr.
     */
    virtual SwitchCommand tick() = 0;
    [[nodiscard]] sf::Time getTimePerTick() const;
    [[nodiscard]] unsigned int getMaxTicksPerFrame() const;
    [[nodiscard]] sf::Color getBackgroundColor() const;
    [[nodiscard]] bool isLagSimulationDebugEnabled() const;
    virtual ~WorldInterface() = default;
};

#endif //OOP_WORLDINTERFACE_H
