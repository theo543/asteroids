#ifndef ASTEROIDS_SWITCHCOMMAND_H
#define ASTEROIDS_SWITCHCOMMAND_H

#include <memory>

class WorldInterface;

enum class SwitchAction {
    CONTINUE, POP, PUSH, REPLACE
};

class SwitchCommand {
private:
    SwitchAction action;
    std::unique_ptr<WorldInterface> nextWorld;
    explicit SwitchCommand(std::unique_ptr<WorldInterface> nextWorld, SwitchAction action);
public:
    [[nodiscard]] SwitchAction getAction() const;
    [[nodiscard]] bool isContinue() const;
    std::unique_ptr<WorldInterface> takeNextWorld();
    friend class SwitchFactory;
};


#endif //ASTEROIDS_SWITCHCOMMAND_H
