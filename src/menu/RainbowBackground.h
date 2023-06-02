#ifndef ASTEROIDS_RAINBOWBACKGROUND_H
#define ASTEROIDS_RAINBOWBACKGROUND_H

#include <string>
#include <memory>
#include "SFML.h"
#include "menu/UIItem.h"

class RainbowBackground : public UIItem {
    sf::VertexArray vertices;
    sf::Clock clock;
public:
    RainbowBackground();
    sf::Vector2f getLayoutSize() override;
    void draw(sf::RenderTarget &window, sf::Vector2f position) override;
};


#endif //ASTEROIDS_RAINBOWBACKGROUND_H
