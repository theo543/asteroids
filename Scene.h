#ifndef OOP_SCENE_H
#define OOP_SCENE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <memory>

/**
 * A Scene is an interface between the main loop and the game logic.
 * The scene must implement drawing to the window, handling events, and ticking the game world.
 * Handling events is called from another thread to avoid blocking the main loop, so there must not be data races
 * between it and the drawing and ticking thread. To exit, signal to the tick function to return a non-empty TickResult.
 * There are also some properties that can be set to indicate timing, background color, and debug options, with inlined getters for performance.
 */
class Scene : sf::NonCopyable {
protected:
    sf::Time timePerTick = sf::seconds(1.f / 20.f);
    unsigned int maxTicksPerFrame = 5;
    sf::Color backgroundColor = sf::Color::Black;
    bool enableLagSimulationDebug = false;
public:
    /**
     * <b>Will be called from rendering thread.</b>
     * It should draw everything to the window.
     */
    virtual void draw(sf::RenderWindow &window) = 0;
    /**
     * <b>Will be called from event polling thread.</b>
     * Note that this is not the main thread, so you can't directly switch the scene from here or close the window.
     * You must use atomic flags or a thread-safe queue to communicate with the rendering thread.
     * Complex event-parsing logic can be done here before sending the result to the rendering thread.
     * TODO: Implement a ComplexScene class that abstracts this away.
     */
    virtual void handleEvent(sf::Event &event) = 0;
    typedef std::optional<std::unique_ptr<Scene>> TickResult;
    /**
     * <b>Will be called from rendering thread.</b>
     * It should update the physics simulation.
     * @returns If it returns a non-empty optional, the scene will be replaced with the one in the optional or exited if the optional is nullptr.
     */
    virtual TickResult tick() = 0;
    [[nodiscard]] inline sf::Time getTimePerTick() const { return timePerTick; }
    [[nodiscard]] inline unsigned int getMaxTicksPerFrame() const { return maxTicksPerFrame; }
    [[nodiscard]] inline sf::Color getBackgroundColor() const { return backgroundColor; }
    [[nodiscard]] inline bool isLagSimulationDebugEnabled() const { return enableLagSimulationDebug; }
    virtual ~Scene() = default;
};

#endif //OOP_SCENE_H
