#ifndef OOP_SWITCHFACTORY_TPP
#define OOP_SWITCHFACTORY_TPP

#include "world/SwitchFactory.h" // just to help the IDE find definitions
#include "type_not_of_claimed_base.h"

SwitchCommand SwitchFactory::fullscreen_on() {
    return SwitchCommand(nullptr, FullScreenCommand::ON, SwitchAction::CONTINUE);
}
SwitchCommand SwitchFactory::fullscreen_off() {
    return SwitchCommand(nullptr, FullScreenCommand::OFF, SwitchAction::CONTINUE);
}
SwitchCommand SwitchFactory::fullscreen_toggle() {
    return SwitchCommand(nullptr, FullScreenCommand::TOGGLE, SwitchAction::CONTINUE);
}
SwitchCommand SwitchFactory::empty() {
    return SwitchCommand(nullptr, SwitchAction::CONTINUE);
}
SwitchCommand SwitchFactory::pop() {
    return SwitchCommand(nullptr, SwitchAction::POP);
}
template <typename T, typename... Args>
SwitchCommand SwitchFactory::push(Args&&... args) {
    return SwitchCommand(std::make_unique<T>(std::forward<Args>(args)...), SwitchAction::PUSH);
}
template <typename T, typename... Args>
SwitchCommand SwitchFactory::replace(Args&&... args) {
    return SwitchCommand(std::make_unique<T>(std::forward<Args>(args)...), SwitchAction::REPLACE);
}
template <typename T>
void SwitchFactory::validate_type(const std::unique_ptr<WorldInterface> &world) {
    if(dynamic_cast<T*>(world.get()) == nullptr)
        throw type_not_of_claimed_base(typeid(T), world.get());
}
template <typename T>
SwitchCommand SwitchFactory::push(std::unique_ptr<WorldInterface> world) {
    validate_type<T>(world);
    return SwitchCommand(std::move(world), SwitchAction::PUSH);
}
template <typename T>
SwitchCommand SwitchFactory::replace(std::unique_ptr<WorldInterface> world) {
    validate_type<T>(world);
    return SwitchCommand(std::move(world), SwitchAction::REPLACE);
}

#endif //OOP_SWITCHFACTORY_TPP
