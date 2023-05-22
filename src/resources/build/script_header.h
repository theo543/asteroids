#ifndef ASTEROIDS_SCRIPT_HEADER_H
#define ASTEROIDS_SCRIPT_HEADER_H

#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <inja/inja.hpp>
#include <utility>

/**
 * These libraries take annoyingly long to compile usually, probably because of all the templates in them.
 * Moved the includes into this header file so CMake can precompile it.
 */


#endif //ASTEROIDS_SCRIPT_HEADER_H
