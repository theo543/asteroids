#ifndef ASTEROIDS_TYPE_NOT_OF_CLAIMED_BASE_H
#define ASTEROIDS_TYPE_NOT_OF_CLAIMED_BASE_H

#include "main/game_exception.h"
#include <typeinfo>

class WorldInterface;

class type_not_of_claimed_base : public game_exception {
public:
    explicit type_not_of_claimed_base(const std::type_info &claimed_type, const WorldInterface *actual_object);
};


#endif //ASTEROIDS_TYPE_NOT_OF_CLAIMED_BASE_H
