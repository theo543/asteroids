#include <span>
#include <unordered_map>
#include "resources/EmbeddedAccessor.h"
#include "resources/ResourceBuilder.h"
using RawByte = EmbeddedAccessor::RawByte;
using RawBytes = EmbeddedAccessor::RawBytes;
using EmbeddedResource = EmbeddedAccessor::EmbeddedResource;
typedef std::unordered_map<std::string, EmbeddedResource> EmbeddedMap;
namespace {
## for resource in resources
    constexpr RawByte {{ resource.id }}_ARRAY[] = {{ hex_init_list(resource.path) }};
    constexpr RawBytes {{ resource.id }}_SPAN { {{ resource.id }}_ARRAY, {{ resource.id }}_ARRAY + sizeof({{ resource.id }}_ARRAY) };
## endfor
    const EmbeddedMap embedded_map = []() {
        EmbeddedMap tmp_map;
        ResourceBuilder builder{};
## for resource in resources
## for property in schema
        {% if property.1 == "string" %}builder.{{ property.0 }}( R"STRING_LITERAL( {{ at(resource, property.0) }} )STRING_LITERAL" );
        {% else if property.1 == "number" %}builder.{{ property.0 }}({{ int(at(resource, property.0)) }});
        {% else if property.1 == "RawBytes" %}builder.{{ property.0 }}({{ resource.id }}_SPAN);
        {% else %}{{ error("Unknown field type" + property.1) }}{% endif %}
## endfor
        tmp_map["{{ resource.id }}"] = builder.takeBuiltResource();
## endfor
        return tmp_map;
    }();
}
const EmbeddedResource *EmbeddedAccessor::lookupEmbeddedResource(const std::string& id) {
    auto iter = embedded_map.find(id);
    if(iter == embedded_map.end())
        return nullptr;
    else return &iter->second;
}
