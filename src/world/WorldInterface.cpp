#include "world/WorldInterface.h"

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
