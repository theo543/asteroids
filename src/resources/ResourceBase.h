#ifndef ASTEROIDS_RESOURCEBASE_H
#define ASTEROIDS_RESOURCEBASE_H

#include <unordered_map>
#include <span>
#include <string>

class EmbeddedResourceAccessor {
protected:
    typedef std::span<const unsigned char> RawBytes;
    struct EmbeddedResource {
        RawBytes bytes;
    };
    [[nodiscard]] static const std::unordered_map<std::string, EmbeddedResource> &getEmbeddedResources();
};

#endif //ASTEROIDS_RESOURCEBASE_H
