#include "ui_item_not_found.h"
#include <menu/UIItem.h>

namespace {
    std::string assemble_string(const std::shared_ptr<UIItem> &bad_item) {
        std::string type = "nullptr";
        if(bad_item) {
            [[maybe_unused]] const auto &bad_item_ref = *bad_item;
            type = typeid(bad_item_ref).name();
        }
        return "Failed to find item located at address " + std::to_string(reinterpret_cast<std::intptr_t>(bad_item.get())) + " of type " + type
        + " in UI. Item may have come from another UI.";
    }
}

ui_item_not_found::ui_item_not_found(const std::shared_ptr<UIItem> &bad_item) : game_exception(assemble_string(bad_item)) {}
