#include <fstream>
#include <filesystem>
#include <iostream>

///TODO placeholder!
constexpr auto placeholder = R"(
    #include "resources/EmbeddedAccessor.h"
    typedef EmbeddedAccessor RB;
    const RB::EmbeddedMap placeholder = {};
    const RB::EmbeddedMap &RB::getEmbeddedResources() {
        return placeholder;
    }
)";

int main(int argc, char **argv) {
    if(argc < 2) return 1;
    std::ofstream file(argv[1]);
    file << placeholder;
}
