#ifndef OOP_UI_H
#define OOP_UI_H

#include <atomic_queue/atomic_queue.h>
#include <cs_plain_guarded.h>
#include <functional>
#include <optional>
#include <set>
#include <vector>
#include "menu/UIItem.h"
#include "world/WorldInterface.h"
#include "world/SwitchFactory.h"

namespace sf { class RenderWindow; }

/**
 * Very simple UI class.
 * Menu items will be laid out in a column, other elements handle their own layout.
 * No nested nodes or anything like we have in HTML.
 */
class UI final : sf::NonCopyable {
public:
    enum class HideBehavior {
        Ignore,
        Hide,
        Exit
    };
private:
   atomic_queue::AtomicQueue2<sf::Event, 128> eventQueue;
   const static inline std::set<sf::Event::EventType> interestingEventsDefault = {sf::Event::EventType::KeyPressed, sf::Event::EventType::Closed};
   libguarded::plain_guarded<std::set<sf::Event::EventType>> interestingEvents = interestingEventsDefault;
   std::optional<sf::Keyboard::Key> hideKey, forwardKey, backKey;
   SwitchCommand nextTransition = SwitchFactory::empty();
   std::vector<std::shared_ptr<UIItem>> items;
   std::shared_ptr<UIItem> selectedItem = nullptr;
   bool open = true;
   void updateInterestingEvents();
   void updateSelected(const std::shared_ptr<UIItem> &newSelected);
   bool internalHandleEvent(const sf::Event &event);
   std::function<SwitchCommand()> exitHandler;
   HideBehavior hide = HideBehavior::Ignore;
   SwitchCommand safeInvokeExitHandler();
   std::vector<std::weak_ptr<UIItem>> tickWatchers;
public:
    void update(sf::RenderWindow &window);
    void notifyTick();
    void pollingThreadHandleEvent(sf::Event &event);
    void setHideKey(std::optional<sf::Keyboard::Key> key);
    void setForwardKey(std::optional<sf::Keyboard::Key> key);
    void setBackKey(std::optional<sf::Keyboard::Key> key);
    void setOpen(bool open);
    [[nodiscard]] bool isOpen() const;
    void addItem(const std::shared_ptr<UIItem>& item);
    [[maybe_unused]] void removeItem(const std::shared_ptr<UIItem>& item);
    [[nodiscard]] bool hasTransition() const;
    SwitchCommand getNextTransition();
    void setExitHandler(std::function<SwitchCommand()> exitHandler);
    void setHideBehavior(HideBehavior hide);
    void forAllSetPixelAlign(bool pixelAlign);
};


#endif //OOP_UI_H
