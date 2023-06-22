#include "SFML.h"
#include "resources/GlobalLoaders.h"
#include "resources/OnDemandLoader.h"
#include "resources/singleton_instance_error.h"

GlobalLoaders *GlobalLoaders::instance = nullptr;

GlobalLoaders::GlobalLoaders() {
    if(instance) throw singleton_instance_error("GlobalLoaders");
    instance = this;
}

GlobalLoaders::~GlobalLoaders() {
    instance = nullptr;
}

ResourceLoader<sf::SoundBuffer> &GlobalLoaders::SoundBuffers() {
    if(!instance) throw singleton_instance_error("GlobalLoaders");
    if(!instance->sounds) instance->sounds = std::make_unique<OnDemandLoader<sf::SoundBuffer>>();
    return *instance->sounds;
}

ResourceLoader<sf::Font> &GlobalLoaders::Fonts() {
    if(!instance) throw singleton_instance_error("GlobalLoaders");
    if(!instance->fonts) instance->fonts = std::make_unique<OnDemandLoader<sf::Font>>();
    return *instance->fonts;
}
