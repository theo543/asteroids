#ifndef ASTEROIDS_RESOURCELOADER_H
#define ASTEROIDS_RESOURCELOADER_H

#include "resources/ResourceBase.h"

template <typename ResourceType>
class ResourceLoader : protected EmbeddedResourceAccessor {
public:
    virtual const ResourceType &load(const std::string &name) = 0;
    virtual ~ResourceLoader() = default;
};


#endif //ASTEROIDS_RESOURCELOADER_H
