#ifndef OOP_SCENE_H
#define OOP_SCENE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <optional>

class Scene : sf::NonCopyable {
protected:
    // subclasses should set these properties as needed
    // these can't use virtual functions because that'd be multiple virtual calls per frame
    sf::Time timePerTick = sf::seconds(1.f / 20.f);
    unsigned int maxTicksPerFrame = 5;
    sf::Color backgroundColor = sf::Color::Black;
public:
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual bool handleEvent(sf::Event &event) = 0;
    typedef std::optional<std::unique_ptr<Scene>> TickResult;
    virtual TickResult tick() = 0;
    [[nodiscard]] inline sf::Time getTimePerTick() const { return timePerTick; }
    [[nodiscard]] inline unsigned int getMaxTicksPerFrame() const { return maxTicksPerFrame; }
    [[nodiscard]] inline sf::Color getBackgroundColor() const { return backgroundColor; }
    virtual ~Scene() = default;
};

#endif //OOP_SCENE_H
