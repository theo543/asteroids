#ifndef OOP_WORLDINTERFACE_H
#define OOP_WORLDINTERFACE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <memory>

/**
 * A WorldInterface is an interface between MainLoop and the game logic.
 * The main loop calls functions for drawing to the window, handling events, and ticking the game world.
 * Handling events is called from another thread to avoid blocking the main loop, so there must not be data races
 * between it and the drawing and ticking thread. To exit, signal to the tick function to return a non-empty TickResult.
 * There are also some properties that can be set to indicate timing, background color, and debug options, with inlined getters for performance.
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
    explicit WorldInterface(bool enableLagSimulationDebug = false) : enableLagSimulationDebug(enableLagSimulationDebug) {}
    /// Because maxTicksPerFrame should be set based on timePerTick and the lowest expected FPS, they are both set in the same function.
    inline void setTiming(sf::Time t_t, unsigned int max_t) { timePerTick = t_t; maxTicksPerFrame = max_t; }
    inline void setBackgroundColor(sf::Color color) { this->backgroundColor = color; }
public:
    /**
     * Called by the main loop before starting the rendering thread.
     * Allows things like changing the title of the window that can't be done from the constructor
     * because a world is constructed before being passed to the main loop.
     */
    virtual void init(sf::RenderWindow &window) = 0;
    virtual void draw(sf::RenderWindow &window) = 0;
    /**
     * <b>Will be called from event polling thread.</b>
     * It is called when an event is received from the window. Unfortunately, it can't mess with the world state since it's called from another thread.
     * Instead, it has to tell the other thread to do it. SFML provides real-time input so only the UI and close/resize events will involve this function.
     */
    virtual void handleEvent(sf::Event &event) = 0;
    typedef std::optional<std::unique_ptr<WorldInterface>> TickResult;
    static inline TickResult CONTINUE() {return std::nullopt;}
    static inline TickResult EXIT() {return nullptr;}
    template <typename T>
    static inline TickResult REPLACE() {return std::make_unique<T>();}
    /**
     * <b>Will be called from rendering thread.</b>
     * It should update the physics simulation.
     * @returns If it returns a non-empty optional, the world will be replaced with the one in the optional or exited if the optional is nullptr.
     */
    virtual TickResult tick() = 0;
    [[nodiscard]] inline sf::Time getTimePerTick() const { return timePerTick; }
    [[nodiscard]] inline unsigned int getMaxTicksPerFrame() const { return maxTicksPerFrame; }
    [[nodiscard]] inline sf::Color getBackgroundColor() const { return backgroundColor; }
    [[nodiscard]] inline bool isLagSimulationDebugEnabled() const { return enableLagSimulationDebug; }
    virtual ~WorldInterface() = default;
};

#endif //OOP_WORLDINTERFACE_H
