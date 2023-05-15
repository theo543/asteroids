#ifndef ASTEROIDS_GAME_EXCEPTION_H
#define ASTEROIDS_GAME_EXCEPTION_H

#include <stdexcept>

class game_exception : public std::logic_error {
    using std::logic_error::logic_error;
protected:
    explicit game_exception(const std::string &whatStr_);
};

#endif //ASTEROIDS_GAME_EXCEPTION_H
