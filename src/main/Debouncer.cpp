#include "Debouncer.h"

const sf::Time Debouncer::delayWithoutRelease = sf::seconds(1);
const sf::Time Debouncer::delayWithRelease = sf::milliseconds(20);

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
