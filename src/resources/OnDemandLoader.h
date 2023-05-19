#ifndef ASTEROIDS_ONDEMANDLOADER_H
#define ASTEROIDS_ONDEMANDLOADER_H

#include "resources/ResourceLoader.h"

template <typename ResourceType>
class OnDemandLoader : public ResourceLoader<ResourceType> {
    std::unordered_map<std::string, ResourceType> loaded;
public:
    const ResourceType &load(const std::string &name) override;
};

#include "resources/OnDemandLoader.tpp"

#endif //ASTEROIDS_ONDEMANDLOADER_H
