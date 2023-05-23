#ifndef ASTEROIDS_EMBEDDEDACCESSOR_H
#define ASTEROIDS_EMBEDDEDACCESSOR_H

#include <unordered_map>
#include <span>
#include <string>

class EmbeddedAccessor {
public:
    typedef const unsigned char RawByte;
    typedef std::span<RawByte> RawBytes;
    struct EmbeddedResource {
        std::string id;
        std::string path;
        RawBytes bytes;
    };
protected:
    [[nodiscard]] const EmbeddedResource *lookupEmbeddedResource(const std::string& id);
};

#endif //ASTEROIDS_EMBEDDEDACCESSOR_H
