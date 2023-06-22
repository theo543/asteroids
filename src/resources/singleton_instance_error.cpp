#include "resources/singleton_instance_error.h"

singleton_instance_error::singleton_instance_error(const std::string &className) :
game_exception("Singleton class " + className + " either has no instance or has multiple instances simultaneously.") {}
