#ifndef ASTEROIDS_GLOBALLOADERS_H
#define ASTEROIDS_GLOBALLOADERS_H

#include "resources/ResourceLoader.h"
#include <memory>

namespace sf { class SoundBuffer; class Font; }

class GlobalLoaders {
private:
    std::unique_ptr<ResourceLoader<sf::SoundBuffer>> sounds;
    std::unique_ptr<ResourceLoader<sf::Font>> fonts;
    static GlobalLoaders *instance;
    GlobalLoaders();
    ~GlobalLoaders();
public:
    friend int main();
    static ResourceLoader<sf::SoundBuffer> &SoundBuffers();
    static ResourceLoader<sf::Font> &Fonts();
};


#endif //ASTEROIDS_GLOBALLOADERS_H
