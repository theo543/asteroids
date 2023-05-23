#include "SFML.h"
#include "resources/GlobalLoaders.h"
#include "resources/OnDemandLoader.h"

std::unique_ptr<ResourceLoader<sf::SoundBuffer>> GlobalLoaders::sounds = nullptr;
std::unique_ptr<ResourceLoader<sf::Font>> GlobalLoaders::fonts = nullptr;

ResourceLoader<sf::SoundBuffer> &GlobalLoaders::SoundBuffers() {
    if(!sounds) sounds = std::make_unique<OnDemandLoader<sf::SoundBuffer>>();
    return *sounds;
}

ResourceLoader<sf::Font> &GlobalLoaders::Fonts() {
    if(!fonts) fonts = std::make_unique<OnDemandLoader<sf::Font>>();
    return *fonts;
}
