#include "ResourceBuilder.h"

ResourceBuilder::ResourceBuilder() : built() {}

void ResourceBuilder::id(std::string id) {
    built.id = std::move(id);
}

void ResourceBuilder::path(std::string path) {
    built.path = std::move(path);
}

void ResourceBuilder::bytes(ResourceBuilder::RawBytes bytes) {
    built.bytes = bytes;
}

using EmbeddedResource = ResourceBuilder::EmbeddedResource;

EmbeddedResource ResourceBuilder::takeBuiltResource() {
    EmbeddedResource ret = {};
    std::swap(ret, built);
    return ret;
}
