#ifndef OOP_SWITCHFACTORY_TPP
#define OOP_SWITCHFACTORY_TPP

#include "world/SwitchFactory.h" // just to help the IDE find definitions

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
        throw std::invalid_argument("Type is not convertible to claimed type");
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
