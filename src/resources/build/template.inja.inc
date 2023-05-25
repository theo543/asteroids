#include <mapbox/eternal.hpp>
#include <span>
#include <unordered_map>
#include <stdexcept>
#include "resources/EmbeddedAccessor.h"
using RawByte = EmbeddedAccessor::RawByte;
using RawBytes = EmbeddedAccessor::RawBytes;
using EmbeddedResource = EmbeddedAccessor::EmbeddedResource;
namespace {
    class ResourceBuilder {
    public:
        consteval ResourceBuilder() = default;
        consteval ResourceBuilder(const ResourceBuilder& other) = default;
        consteval ResourceBuilder id(const char *id) {
            built.id = id;
            return *this;
        }
        consteval ResourceBuilder path(const char *path) {
            built.path = path;
            return *this;
        }
        consteval ResourceBuilder bytes(RawBytes bytes) {
            built.bytes = bytes;
            return *this;
        }
        consteval EmbeddedResource takeBuiltResource() {
            if(built.id || built.path || built.bytes.data())
                return built;
            else throw std::logic_error("EmbeddedResource not fully initialized"); // causes compile error, exception not actually thrown
        }
    private:
        EmbeddedResource built{nullptr, nullptr, RawBytes{(const unsigned char*)nullptr, (std::size_t)0}};
    };
## for resource in resources
    constexpr RawByte {{ resource.id }}_ARRAY[] = {{ hex_init_list(resource.path) }};
    constexpr RawBytes {{ resource.id }}_SPAN { {{ resource.id }}_ARRAY, {{ resource.id }}_ARRAY + sizeof({{ resource.id }}_ARRAY) };
## endfor
    constexpr auto embedded_map = mapbox::eternal::hash_map<mapbox::eternal::string, EmbeddedResource>({
## for resource in resources
        {
        mapbox::eternal::string{R"RESOURCE_ID({{ resource.id }})RESOURCE_ID"},
        ResourceBuilder{}
## for property in schema
        {% if property.1 == "string" %}.{{ property.0 }}( R"STRING_LITERAL( {{ at(resource, property.0) }} )STRING_LITERAL" )
        {% else if property.1 == "number" %}.{{ property.0 }}({{ int(at(resource, property.0)) }})
        {% else if property.1 == "RawBytes" %}.{{ property.0 }}({{ resource.id }}_SPAN)
        {% else %}{{ error("Unknown field type" + property.1) }}{% endif %}
## endfor
        .takeBuiltResource()
        },
## endfor
    });
}
const EmbeddedResource *EmbeddedAccessor::lookupEmbeddedResource(const char *id) {
    auto iter = embedded_map.find(id);
    if(iter == embedded_map.end())
        return nullptr;
    else return &iter->second;
}
const EmbeddedResource *EmbeddedAccessor::lookupEmbeddedResource(const std::string &id) {
    return lookupEmbeddedResource(id.c_str());
}
