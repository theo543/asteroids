#ifndef OOP_SWITCHFACTORY_H
#define OOP_SWITCHFACTORY_H

#include "world/WorldInterface.h"

/// Convenience class for creating SwitchCommand.
class SwitchFactory {
public:
    inline static SwitchCommand empty();
    inline static SwitchCommand pop();
    inline static SwitchCommand fullscreen_on();
    inline static SwitchCommand fullscreen_off();
    inline static SwitchCommand fullscreen_toggle();
    template <typename T = WorldInterface, typename... Args>
    static SwitchCommand push(Args&&... args);
    template <typename T = WorldInterface, typename... Args>
    static SwitchCommand replace(Args&&... args);
    template <typename T = WorldInterface>
    static SwitchCommand push(std::unique_ptr<WorldInterface> world);
    template <typename T = WorldInterface>
    static SwitchCommand replace(std::unique_ptr<WorldInterface> world);
private:
    template <typename T>
    static void validate_type(const std::unique_ptr<WorldInterface> &world);
};

#include "world/SwitchFactory.tpp"

#endif //OOP_SWITCHFACTORY_H
