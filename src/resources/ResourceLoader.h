#ifndef ASTEROIDS_RESOURCELOADER_H
#define ASTEROIDS_RESOURCELOADER_H

#include "resources/EmbeddedAccessor.h"

template <typename ResourceType>
class ResourceLoader : protected EmbeddedAccessor {
public:
    virtual const ResourceType &load(const std::string &name) = 0;
    virtual ~ResourceLoader() = default;
};


#endif //ASTEROIDS_RESOURCELOADER_H
