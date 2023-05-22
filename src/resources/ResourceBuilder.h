#ifndef ASTEROIDS_RESOURCEBUILDER_H
#define ASTEROIDS_RESOURCEBUILDER_H

#include "resources/EmbeddedAccessor.h"

/**
 * Builds an EmbeddedResource struct by calling methods named after member data.
 * Enables adding new resource properties without recompiling build script or depending on constructor parameter order.
 * Methods on this class should match schema.json file.
 * NOTE: RawBytes is just a span to static read-only data, actual embedding is handled by resources/build/script.cpp script.
 */
class [[maybe_unused]] ResourceBuilder {
public:
    using EmbeddedResource = EmbeddedAccessor::EmbeddedResource;
    using RawBytes = EmbeddedAccessor::RawBytes;
    ResourceBuilder();
    void id(std::string);
    void path(std::string);
    void bytes(RawBytes);
    EmbeddedResource takeBuiltResource();
private:
    EmbeddedResource built;
};


#endif //ASTEROIDS_RESOURCEBUILDER_H
