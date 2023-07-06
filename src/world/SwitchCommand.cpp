#include "SwitchCommand.h"
#include "world/WorldInterface.h"

SwitchCommand::SwitchCommand(std::unique_ptr<WorldInterface> nextWorld, FullScreenCommand fullscreen, SwitchAction action) : action(action), fullscreen(fullscreen), nextWorld(std::move(nextWorld)) {}

SwitchCommand::SwitchCommand(std::unique_ptr<WorldInterface> nextWorld, SwitchAction action) : SwitchCommand(std::move(nextWorld), FullScreenCommand::NONE, action) {}

FullScreenCommand SwitchCommand::getFullscreen() const {
    return fullscreen;
}

SwitchAction SwitchCommand::getAction() const {
    return action;
}

bool SwitchCommand::isEmpty() const {
    return action == SwitchAction::CONTINUE && fullscreen == FullScreenCommand::NONE;
}

std::unique_ptr<WorldInterface> SwitchCommand::takeNextWorld() {
    auto ret = std::move(nextWorld);
    action = SwitchAction::CONTINUE; // since unique_ptr is moved, this command is now empty
    return ret;
}
