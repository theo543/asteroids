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

bool files_equal(const std::filesystem::path &path1, const std::filesystem::path &path2) {
    auto mode = std::ios::binary | std::ios::ate; // open file in binary mode and seek to the end
    std::ifstream file1(path1, mode), file2(path2, mode);
    if(!file1.is_open() || !file2.is_open()) return false;
    if(file1.tellg() != file2.tellg()) return false; // check file size
    file1.seekg(0, std::ios::beg); // done checking file size, rewind to the beginning
    file2.seekg(0, std::ios::beg);
    std::istreambuf_iterator<char> begin1(file1), begin2(file2);
    std::istreambuf_iterator<char> end;
    return std::equal(begin1, end, begin2);
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

    std::vector<std::filesystem::directory_entry> sorted_resources;
    std::copy_if(std::filesystem::directory_iterator{resources_dir}, std::filesystem::directory_iterator{}, std::back_inserter(sorted_resources), [](const auto &dir_entry) {
        return dir_entry.is_regular_file() && dir_entry.path().extension() == ".json";
    });
    // ensure determinism, OS can return files in any order
    std::sort(sorted_resources.begin(), sorted_resources.end(), [](const auto &a, const auto &b) {
        return a.path().filename().string() < b.path().filename().string();
    });

    for(const auto &dir_entry : sorted_resources) {
        std::ifstream file(dir_entry.path());
        if(file) {
            json res_json;
            file >> res_json;
            for(const auto &prop : schema_data["schema"]) {
                if(prop[1] == "string") { if (!res_json[prop[0]].is_string()) error("Property " + prop[0].get<std::string>() + " should be string"); }
                else if(prop[1] == "number") { if(!res_json[prop[0]].is_number()) error("Property " + prop[0].get<std::string>() + " should be number"); }
                else if(prop[1] == "RawBytes") { /* no check needed for RawBytes, hex_init_list will error if the file is missing */ }
                else error("Unknown property type " + prop[1].get<std::string>() + " defined in schema");
            }
            res_json["path"] = (resources_dir / std::filesystem::path(res_json["path"].get<std::string>())).string();
            template_data["resources"].push_back(res_json);
        } else error("Failed to open resource " + dir_entry.path().string());
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
        else error("Unknown error - error message is not a string");
    });
    inja::Template temp = template_env.parse_file(template_file.string());
    try {
        auto output_temp_file = [output_file]() mutable {return output_file.replace_extension(output_file.extension().string() + ".tmp");}();
        template_env.write(temp, template_data, output_temp_file.string());
        // build systems rely on modification time, don't overwrite if the file is the same
        if(!files_equal(output_file, output_temp_file)) {
            std::filesystem::rename(output_temp_file, output_file); // atomic overwrite
        } else {
            std::filesystem::remove(output_temp_file);
        }
    } catch (inja::InjaError &err) {
        error(std::string{"Failed to render template. InjaError: "} + err.what());
    }

    return EXIT_SUCCESS;
}
