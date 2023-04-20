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

WorldInterface::WorldInterface(bool enableLagSimulationDebug) : enableLagSimulationDebug(enableLagSimulationDebug) {}

void WorldInterface::setTiming(sf::Time t_t, unsigned int max_t) {
    timePerTick = t_t;
    maxTicksPerFrame = max_t;
}

void WorldInterface::setBackgroundColor(sf::Color color) {
    backgroundColor = color;
}

sf::Time WorldInterface::getTimePerTick() const {
    return timePerTick;
}

unsigned int WorldInterface::getMaxTicksPerFrame() const {
    return maxTicksPerFrame;
}

sf::Color WorldInterface::getBackgroundColor() const {
    return backgroundColor;
}

bool WorldInterface::isLagSimulationDebugEnabled() const {
    return enableLagSimulationDebug;
}
