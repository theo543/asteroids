#include "SwitchCommand.h"
#include "world/WorldInterface.h"

SwitchCommand::SwitchCommand(std::unique_ptr<WorldInterface> nextWorld, SwitchAction action) : action(action), nextWorld(std::move(nextWorld)) {}

SwitchAction SwitchCommand::getAction() const {
    return action;
}

bool SwitchCommand::isContinue() const {
    return action == SwitchAction::CONTINUE;
}

std::unique_ptr<WorldInterface> SwitchCommand::takeNextWorld() {
    auto ret = std::move(nextWorld);
    action = SwitchAction::CONTINUE; // since unique_ptr is moved, this command is now empty
    return ret;
}