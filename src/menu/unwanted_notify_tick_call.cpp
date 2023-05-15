#include "unwanted_notify_tick_call.h"
#include "menu/UIItem.h"

unwanted_notify_tick_call::unwanted_notify_tick_call(const UIItem &callee) : game_exception(
        callee.wantsTickNotifications() ?
        "UIItem::notifyTick() called incorrectly, however item now claims to want tick notifications. notifyTicks private variable should not be modified."
        : "UIItem::notifyTick() called incorrectly."
        ) {}
