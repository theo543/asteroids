#ifndef ASTEROIDS_GLOBALLOADERS_H
#define ASTEROIDS_GLOBALLOADERS_H

#include "resources/ResourceLoader.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class GlobalLoaders {
private:
    static std::unique_ptr<ResourceLoader<sf::SoundBuffer>> sounds;
    static std::unique_ptr<ResourceLoader<sf::Font>> fonts;
public:
    GlobalLoaders() = delete;
    static ResourceLoader<sf::SoundBuffer> &SoundBuffers();
    static ResourceLoader<sf::Font> &Fonts();
};


#endif //ASTEROIDS_GLOBALLOADERS_H
