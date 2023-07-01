#include <numeric>
#include <cmath>
#include "UI.h"
#include "world/SwitchFactory.h"
#include "menu/ui_item_not_found.h"

SwitchCommand UI::safeInvokeExitHandler() {
    if(exitHandler) return exitHandler();
    else return SwitchFactory::pop();
}

void move_if_free(SwitchCommand &current, SwitchCommand &&next) {
    if(current.isContinue() && !next.isContinue())
        current = std::move(next);
}

bool UI::internalHandleEvent(const sf::Event &event) {
    if(event.type == sf::Event::Closed) {
        auto r = safeInvokeExitHandler();
        move_if_free(nextTransition, std::move(r));
        return true;
    }
    if(event.type != sf::Event::KeyPressed)
        return false;
    if(hideKey.has_value() && event.key.code == hideKey.value()) {
        switch(hide) {
            case HideBehavior::Hide:
                open = !open;
                break;
            case HideBehavior::Ignore:
                break;
            case HideBehavior::Exit:
                auto r = safeInvokeExitHandler();
                move_if_free(nextTransition, std::move(r));
                break;
        }
        return true;
    }
    enum {none, forward, back} dir = none;
    if(forwardKey.has_value() && event.key.code == forwardKey.value())
        dir = forward;
    if(backKey.has_value() && event.key.code == backKey.value())
        dir = back;
    if(dir) {
        std::vector<std::shared_ptr<UIItem>> selectableItems;
        std::copy_if(items.begin(), items.end(), std::back_inserter(selectableItems), [](const auto &item) {
            return item->isSelectable();
        });
        if(selectableItems.empty()) {
            updateSelected(nullptr);
        } else {
            auto it = std::find(selectableItems.begin(), selectableItems.end(), selectedItem);
            if (it == selectableItems.end()) {
                updateSelected(selectableItems.front());
            } else {
                if (dir == forward) {
                    it++;
                    if (it == selectableItems.end())
                        it = selectableItems.begin();
                } else {
                    if (it == selectableItems.begin())
                        it = selectableItems.end();
                    it--;
                }
                updateSelected(*it);
            }
        }
        return true;
    }
    return false;
}

void UI::updateSelected(const std::shared_ptr<UIItem> &newSelected) {
    if(selectedItem) {
        selectedItem->deselected();
    }
    selectedItem = newSelected;
    if(selectedItem) {
        selectedItem->selected();
    }
}

void UI::notifyTick() {
    for(std::size_t x = 0;x<tickWatchers.size();x++) {
        auto item = tickWatchers[x].lock();
        if(item) {
            item->notifyTick();
        } else {
            swap(tickWatchers[x], tickWatchers.back());
            tickWatchers.pop_back();
            x--;
        }
    }
}

void UI::update(sf::RenderWindow &window) {
    sf::Event e{};
    while(eventQueue.try_pop(e)) {
        if(!internalHandleEvent(e)) {
            if(hasTransition())
                continue; // Don't let the UI handle events if we're transitioning
            for(auto &item : items) {
                auto result = item->handleEvent(e);
                move_if_free(nextTransition, std::move(result));
            }
        }
    }
    std::vector<UIItem*> menuItems;
    for(auto &item : items) {
        if(open || item->isAlwaysVisible()) {
            if(item->isMenuItem()) {
                menuItems.push_back(item.get());
            } else {
                item->draw(window, item->getFixedPosition().value());
            }
        }
    }
    float totalHeight = std::accumulate(menuItems.begin(), menuItems.end(), 0.0f, [](float acc, UIItem *item) {
        return acc + item->getLayoutSize().y + item->getMarginTop() + item->getMarginBottom();
    });
    float margin = (static_cast<float>(window.getSize().y) - totalHeight) / 2.0f;
    float y = margin;
    for(auto &item : menuItems) {
        y += item->getMarginTop();
        float x = (static_cast<float>(window.getSize().x) - item->getLayoutSize().x) / 2.0f;
        if(item->getPixelAlign()) {
            x = std::round(x);
            y = std::round(y);
        }
        item->draw(window, {x, y});
        y += item->getLayoutSize().y;
        y += item->getMarginBottom();
    }
}

void UI::pollingThreadHandleEvent(sf::Event &event) {
    {
        auto ie = interestingEvents.lock();
        if (!ie->contains(event.type))
            return;
    }
    eventQueue.push(event);
}

void UI::setHideKey(std::optional<sf::Keyboard::Key> key) {
    hideKey = key;
}

void UI::setForwardKey(std::optional<sf::Keyboard::Key> key) {
    forwardKey = key;
}

void UI::setBackKey(std::optional<sf::Keyboard::Key> key) {
    backKey = key;
}

void UI::setOpen(bool open_) {
    UI::open = open_;
}

[[maybe_unused]] bool UI::isOpen() const {
    return open;
}

void UI::addItem(const std::shared_ptr<UIItem>& item) {
    items.push_back(item);
    if(item->wantsTickNotifications())
        tickWatchers.emplace_back(item);
    if(!item->getInterestingEvents().empty())
        updateInterestingEvents();
}

[[maybe_unused]] void UI::removeItem(const std::shared_ptr<UIItem>& item) { //cppcheck-suppress unusedFunction
    auto it = std::find(items.begin(), items.end(), item);
    if(it != items.end()) {
        items.erase(it);
        if(!item->getInterestingEvents().empty())
            updateInterestingEvents();
        if(selectedItem == item) {
            selectedItem = nullptr;
        }
    } else throw ui_item_not_found(item);
}

void UI::updateInterestingEvents() {
    auto ie = interestingEvents.lock();
    *ie = interestingEventsDefault;
    for(const auto &item : items) {
        for(const auto &e : item->getInterestingEvents()) {
            ie->insert(e);
        }
    }
}

bool UI::hasTransition() const {
    return !nextTransition.isContinue();
}

SwitchCommand UI::getNextTransition() {
    auto r = std::move(nextTransition);
    nextTransition = SwitchFactory::empty();
    return r;
}

void UI::setExitHandler(std::function<SwitchCommand()> exitHandler_) {
    exitHandler = std::move(exitHandler_);
}

void UI::setHideBehavior(UI::HideBehavior hide_) {
    hide = hide_;
}

void UI::forAllSetPixelAlign(bool pixelAlign) {
    for(auto &item : items) {
        item->setPixelAlign(pixelAlign);
    }
}
