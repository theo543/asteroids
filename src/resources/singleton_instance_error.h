#ifndef ASTEROIDS_SINGLETON_INSTANCE_ERROR_H
#define ASTEROIDS_SINGLETON_INSTANCE_ERROR_H

#include "main/game_exception.h"


class singleton_instance_error : public game_exception {
public:
    explicit singleton_instance_error(const std::string &className);
};


#endif //ASTEROIDS_SINGLETON_INSTANCE_ERROR_H
