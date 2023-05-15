#ifndef ASTEROIDS_UI_ITEM_NOT_FOUND_H
#define ASTEROIDS_UI_ITEM_NOT_FOUND_H

#include <memory>
#include "main/game_exception.h"

class UIItem;

class ui_item_not_found : public game_exception {
public:
    explicit ui_item_not_found(const std::shared_ptr<UIItem> &bad_item);
};


#endif //ASTEROIDS_UI_ITEM_NOT_FOUND_H
