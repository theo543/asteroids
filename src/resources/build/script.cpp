#include <fstream>
#include <filesystem>
#include <iostream>

///TODO placeholder!
constexpr auto placeholder =
        "#include \"resources/EmbeddedAccessor.h\"\n"
        "typedef EmbeddedAccessor RB;\n"
        "const RB::EmbeddedMap placeholder = {};"
        "const RB::EmbeddedMap &RB::getEmbeddedResources() {\n"
        "    return placeholder;\n"
        "}\n";

int main(int argc, char **argv) {
    if(argc < 2) return 1;
    std::ofstream file(argv[1]);
    file << placeholder;
}
