#ifndef ASTEROIDS_EMBEDDEDACCESSOR_H
#define ASTEROIDS_EMBEDDEDACCESSOR_H

#include <unordered_map>
#include <span>
#include <string>

class EmbeddedAccessor {
public:
    typedef std::span<const unsigned char> RawBytes;
    struct EmbeddedResource {
        ///TODO metadata
        RawBytes bytes;
    };
    typedef std::unordered_map<std::string, EmbeddedResource> EmbeddedMap;
protected:
    [[nodiscard]] static const EmbeddedMap &getEmbeddedResources();
};

#endif //ASTEROIDS_EMBEDDEDACCESSOR_H
