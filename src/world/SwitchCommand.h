#ifndef ASTEROIDS_SWITCHCOMMAND_H
#define ASTEROIDS_SWITCHCOMMAND_H

#include <memory>

class WorldInterface;

enum class SwitchAction {
    CONTINUE, POP, PUSH, REPLACE
};

enum FullScreenCommand {
    NONE, ON, OFF, TOGGLE
};

class SwitchCommand {
private:
    SwitchAction action;
    FullScreenCommand fullscreen;
    std::unique_ptr<WorldInterface> nextWorld;
    explicit SwitchCommand(std::unique_ptr<WorldInterface> nextWorld, SwitchAction action);
    explicit SwitchCommand(std::unique_ptr<WorldInterface> nextWorld, FullScreenCommand fullscreen, SwitchAction action);
public:
    [[nodiscard]] FullScreenCommand getFullscreen() const;
    [[nodiscard]] SwitchAction getAction() const;
    [[nodiscard]] bool isEmpty() const;
    std::unique_ptr<WorldInterface> takeNextWorld();
    friend class SwitchFactory;
};


#endif //ASTEROIDS_SWITCHCOMMAND_H
