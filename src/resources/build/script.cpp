#include "script_header.h"

using json = nlohmann::json;

void error(const std::string &err) {
    std::cerr << err;
    exit(EXIT_FAILURE);
}

void require_file(const std::filesystem::path &path, const std::string &err_string) {
    if((!exists(path) || is_directory(path))) error(err_string);
}

void require_dir(const std::filesystem::path &path, const std::string &err_string) {
    if((!exists(path) || !is_directory(path))) error(err_string);
}


constexpr char hex_values[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

int main(int argc, char **argv) {
    if(argc != 4 + 1) error("This script takes exactly 4 args: <output_file> <resources_dir> <schema_file> <template_file>");
    std::filesystem::path output_file = argv[1], resources_dir = argv[2], schema_file = argv[3], template_file = argv[4];
    if(is_directory(output_file)) error("Output file is a directory");
    require_dir(resources_dir, "Resources dir missing\n");
    require_file(schema_file, "Schema file missing\n");
    require_file(template_file, "Template file missing\n");
    json schema_data;
    {
        std::ifstream file(schema_file);
        file >> schema_data;
        if(!schema_data["schema"].is_array()) error("Schema should contains properties array");
        for(json prop : schema_data["schema"]) {
            if(!prop.is_array() || prop.size() != 2 || !prop[0].is_string() || !prop[1].is_string()) error("Properties should be pairs of string");
        }
    }
    json template_data;
    template_data["resources"] = json::array();
    template_data["schema"] = schema_data["schema"];
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
    template_env.add_void_callback("error", 1, [](inja::Arguments &args) -> void {
        if(args[0]->is_string())
            error(args[0]->get<std::string>());
        else error("Unknown error");
    });
    inja::Template temp = template_env.parse_file(template_file.string());
    try {
        template_env.write(temp, template_data, output_file.string());
    } catch (inja::InjaError &err) {
        error(std::string{"Failed to render template. InjaError: "} + err.what());
    }

    return EXIT_SUCCESS;
}
