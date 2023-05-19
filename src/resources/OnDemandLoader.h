#ifndef ASTEROIDS_ONDEMANDLOADER_H
#define ASTEROIDS_ONDEMANDLOADER_H

#include "resources/ResourceLoader.h"
#include <concepts>

template <typename SFMLLike>
concept SFMLLikeResource = requires(SFMLLike resource, EmbeddedAccessor::RawBytes resource_data) {
    resource.loadFromMemory(resource_data.data(), resource_data.size()); // the type must have this method
} && std::is_default_constructible_v<SFMLLike>;


template <SFMLLikeResource ResourceType>
class OnDemandLoader : public ResourceLoader<ResourceType> {
    std::unordered_map<std::string, ResourceType> loaded;
public:
    const ResourceType &load(const std::string &name) override;
};

#include "resources/OnDemandLoader.tpp"

#endif //ASTEROIDS_ONDEMANDLOADER_H
