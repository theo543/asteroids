#include <cmath>
#include <bezier.h>
#include "menu/RainbowBackground.h"

RainbowBackground::RainbowBackground() : vertices(sf::TriangleFan, 6) {
    setFixedPosition(sf::Vector2f{0, 0});
}

sf::Vector2f RainbowBackground::getLayoutSize() { return {0.f, 0.f}; }

/// equivalent to CSS ease-in-out
static const Bezier::Bezier<3> easing({
    {0.0f, 0.0f},
    {0.42f, 0.0f},
    {0.58f, 1.0f},
    {1.0f, 1.0f}
});

void RainbowBackground::draw(sf::RenderTarget &window, sf::Vector2f) {
    auto winX = static_cast<float>(window.getSize().x);
    auto winY = static_cast<float>(window.getSize().y);
    float position = std::fmod(clock.getElapsedTime().asSeconds() / 4.f, 4.f);
    const auto ease = [&position](){
        position = easing.valueAt(position, 1);
    };
    if(position < 1.f) {
        ease();
        vertices[0].position = {position * winX, 0};
    } else if(position < 2.f) {
        position -= 1.f;
        ease();
        vertices[0].position = {winX, position * winY};
    } else if(position < 3.f) {
        position -= 2.f;
        ease();
        vertices[0].position = {winX - position * winX, winY};
    } else {
        position -= 3.f;
        ease();
        vertices[0].position = {0, winY - position * winY};
    }
    vertices[1].position = {winX, 0};
    vertices[2].position = {winX, winY};
    vertices[3].position = {0, winY};
    vertices[4].position = {0, 0};
    vertices[5].position = {winX, 0};
    const sf::Uint8 alpha = 130;
    vertices[0].color = {30, 255, 90, 200};
    vertices[1].color = {255, 255, 0, alpha};
    vertices[2].color = {0, 255, 0, alpha};
    vertices[3].color = {0, 255, 255, alpha};
    vertices[4].color = {0, 0, 255, alpha};
    vertices[5].color = vertices[1].color;
    window.draw(vertices);
}
