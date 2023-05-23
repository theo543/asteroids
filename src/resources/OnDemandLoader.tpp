#ifndef ASTEROIDS_ONDEMANDLOADER_TPP
#define ASTEROIDS_ONDEMANDLOADER_TPP

#include <cassert>
#include <stdexcept>
#include "resources/OnDemandLoader.h"

template <SFMLLikeResource ResourceType>
const ResourceType &OnDemandLoader<ResourceType>::load(const std::string &name) {
    if(loaded.contains(name))
        return loaded[name];
    const EmbeddedAccessor::EmbeddedResource *lookup;
    if((lookup = EmbeddedAccessor::lookupEmbeddedResource(name))) {
        const auto &res = *lookup;
        assert(res.bytes.size() == res.bytes.size_bytes());
        loaded[name].loadFromMemory(res.bytes.data(), res.bytes.size());
        return loaded[name];
    } else throw std::invalid_argument("Resource " + name + " not found.");
}

#endif //ASTEROIDS_ONDEMANDLOADER_TPP
