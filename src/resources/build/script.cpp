#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>

using json = nlohmann::json;

void error(const std::string &err) {
    std::cerr << err;
    exit(EXIT_FAILURE);
}

const char hex_values[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

constexpr auto TEMPLATE = R"(
#include <span>
#include <unordered_map>
#include "resources/EmbeddedAccessor.h"
using RawByte = EmbeddedAccessor::RawByte;
using RawBytes = EmbeddedAccessor::RawBytes;
using EmbeddedMap = EmbeddedAccessor::EmbeddedMap;
namespace {
## for resource in resources
    constexpr RawByte {{ resource.id }}_ARRAY[] = {{ hex_init_list(resource.path) }};
    constexpr RawBytes {{ resource.id }} { {{ resource.id }}_ARRAY, {{ resource.id }}_ARRAY + sizeof({{ resource.id }}_ARRAY) };
## endfor
    const EmbeddedMap res_map = []() {
        EmbeddedMap ret_map;
## for resource in resources
        ret_map["{{ resource.id }}"] = { {{ resource.id }} };
## endfor
        return ret_map;
    }();
}
const EmbeddedMap &EmbeddedAccessor::getEmbeddedResources() {
    return res_map;
}
)";

int main(int argc, char **argv) {
    if(argc != 3) error("This script takes 3 args");
    std::filesystem::path output_file = argv[1], resources_dir = argv[2];
    if((!exists(output_file) || is_directory(output_file))) error("Output file missing\n");
    if(!exists(resources_dir) || !is_directory(resources_dir)) error("Source dir missing\n");
    json template_data;
    template_data["resources"] = json::array();
    for(const auto &dir_entry : std::filesystem::directory_iterator{resources_dir}) {
        if(dir_entry.is_regular_file() && dir_entry.path().extension() == ".json") {
            std::ifstream file(dir_entry.path());
            if(file) {
                json res_json;
                file >> res_json;
                if(!(res_json["id"].is_string() && res_json["path"].is_string())) error("JSON properties should be strings");
                res_json["path"] = (resources_dir / std::filesystem::path(res_json["path"].get<std::string>())).string();
                template_data["resources"].push_back(res_json);
            }
        }
    }
    inja::Environment template_env;
    template_env.add_callback("hex_init_list", 1, [](inja::Arguments &args) -> std::string {
        std::string path = args.at(0)->get<std::string>();std::ifstream hex_source{path, std::ios::in | std::ios::binary};
        if(!hex_source) error("Failed to open resource " + path);
        std::string ret = "{";
        for(int byte = hex_source.get();byte != EOF;byte = hex_source.get()) {
            int upper = byte >> 4, lower = byte & 15;
            assert((upper >= 0) && (upper < 16));
            assert((lower >= 0) && (lower < 16));
            ret += "0x";
            ret += hex_values[upper];
            ret += hex_values[lower];
            ret += ",";
        }
        ret += "}";
        return ret;
    });
    inja::Template temp = template_env.parse(TEMPLATE);
    template_env.write(temp, template_data, output_file.string());

    return EXIT_SUCCESS;
}
