#ifndef OOP_DEBOUNCER_H
#define OOP_DEBOUNCER_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <optional>

class Debouncer {
    sf::Clock time;
    std::array<sf::Time, sf::Keyboard::KeyCount> delay;
    /// If a KeyPressed event is repeated, without a KeyReleased event, for the same key, in a time interval below this threshold, then it will be ignored.
    const static sf::Time delayWithRelease;
    const static sf::Time delayWithoutRelease;
public:
    void released(sf::Keyboard::Key);
    /// \return Whether the keypress should be accepted
    bool pressed(sf::Keyboard::Key);
};


#endif //OOP_DEBOUNCER_H
