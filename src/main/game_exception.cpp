#include "game_exception.h"

game_exception::game_exception(const std::string &whatStr_) : std::logic_error(whatStr_) {}
