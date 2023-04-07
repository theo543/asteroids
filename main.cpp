#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <ctime>
#include <sstream>

// To add new embedded resources, update RESOURCES variable in CMakeEmbedResources.cmake and re-run build to regenerate this header
#include "embedded_fwd.h"

const auto PublicPixelFont = []() {
    sf::Font font;
    font.loadFromMemory(PublicPixelTTF.data(), PublicPixelTTF.size());
    return font;
}();

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main() {
    #ifdef __linux__
    XInitThreads();
    #endif

    sf::RenderWindow window;
    // NOTE: sync with env variable APP_WINDOW from .github/workflows/cmake.yml:30
    window.create(sf::VideoMode({800, 700}), "Current Time", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    auto desktop = sf::VideoMode::getDesktopMode();
    // This is safe because screen size isn't nearly as big as 32-bit int. If it is, you have other problems.
    // We also account for the window title bar
    window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2, (desktop.height - window.getSize().y) / 2 - 50)); //NOLINT
    while(window.isOpen()) {
        sf::Event e;//NOLINT this gets initialized in the loop
        while(window.pollEvent(e)) {
            switch(e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                // resize the view to the new size of the window
                window.setView(sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height)));
                break;
            case sf::Event::KeyPressed:
                break;
            default:
                break;
            }
        }

        window.clear(sf::Color(47,79,79, 255));
        sf::Text text;
        text.setFont(PublicPixelFont);
        std::time_t t = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "[%H : %M : %S]");
        text.setString(ss.str());
        text.setCharacterSize(48); // in pixels, not points!
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2);
        float x = window.getView().getSize().x / 2.0f - text.getGlobalBounds().width / 2.0f;
        float y = window.getView().getSize().y / 2.0f - text.getGlobalBounds().height / 2.0f - 2; // Compensate for weird font baseline
        text.setPosition(x, y);
        window.draw(text);
        window.display();
    }

    return 0;
}
