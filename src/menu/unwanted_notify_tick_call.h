#ifndef ASTEROIDS_UNWANTED_NOTIFY_TICK_CALL_H
#define ASTEROIDS_UNWANTED_NOTIFY_TICK_CALL_H

#include "main/game_exception.h"

class UIItem;

class unwanted_notify_tick_call : public game_exception {
public:
    explicit unwanted_notify_tick_call(const UIItem &callee);
};


#endif //ASTEROIDS_UNWANTED_NOTIFY_TICK_CALL_H
