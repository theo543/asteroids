#include "Debouncer.h"

#ifdef __linux__
const sf::Time Debouncer::delayWithoutRelease = sf::seconds(1);
const sf::Time Debouncer::delayWithRelease = sf::milliseconds(100);
#else
const sf::Time Debouncer::delayWithoutRelease = sf::milliseconds(20);
const sf::Time Debouncer::delayWithRelease = sf::milliseconds(10);
#endif

void Debouncer::released(sf::Keyboard::Key key) {
    delay[key] = std::min(delay[key], time.getElapsedTime() + delayWithRelease);
}

bool Debouncer::pressed(sf::Keyboard::Key key) {
    if(delay[key] > time.getElapsedTime()) {
        return false;
    } else {
        delay[key] = time.getElapsedTime() + delayWithoutRelease;
        return true;
    }
}
