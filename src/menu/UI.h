#ifndef OOP_UI_H
#define OOP_UI_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <atomic_queue/atomic_queue.h>
#include <cs_plain_guarded.h>
#include <functional>
#include <optional>
#include <set>
#include <vector>
#include "menu/UIItem.h"
#include "main/WorldInterface.h"

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
   WorldInterface::TickResult nextTransition = std::nullopt;
   std::vector<std::shared_ptr<UIItem>> items;
   std::shared_ptr<UIItem> selectedItem = nullptr;
   bool open = true;
   void updateInterestingEvents();
   void updateSelected(const std::shared_ptr<UIItem> &newSelected);
   bool internalHandleEvent(const sf::Event &event);
   std::function<WorldInterface::TickResult()> exitHandler;
   HideBehavior hide = HideBehavior::Ignore;
public:
    /// Returns true if an exit event was handled.
    void update(sf::RenderWindow &window);
    void pollingThreadHandleEvent(sf::Event &event);
    void setHideKey(std::optional<sf::Keyboard::Key> key);
    void setForwardKey(std::optional<sf::Keyboard::Key> key);
    void setBackKey(std::optional<sf::Keyboard::Key> key);
    void setOpen(bool open);
    [[nodiscard]] bool isOpen() const;
    void addItem(const std::shared_ptr<UIItem>& item);
    [[maybe_unused]] void removeItem(const std::shared_ptr<UIItem>& item);
    [[nodiscard]] bool hasTransition() const;
    WorldInterface::TickResult getNextTransition();
    void setExitHandler(std::function<WorldInterface::TickResult()> exitHandler);
    void setHideBehavior(HideBehavior hide);
};


#endif //OOP_UI_H
