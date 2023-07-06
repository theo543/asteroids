#include "main/MainLoop.h"
#include "main/Menu.h"
#include "resources/GlobalLoaders.h"
#include "asteroids/AsteroidsWorld.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main(int argc, const char **argv) {
    #ifdef __linux__
    XInitThreads();
    #endif

    // putting this in main fixes segfault on exit destructor on Linux
    // segfault happens in GlobalLoaders destructor -> sf::Font -> sf::Texture -> sf::GlContext::acquireTransientContext():411 -> access to thread local
    // maybe on Linux static destruction happens in a different thread?
    GlobalLoaders singletonInstance;

    bool screensaverMode = false;

    {
        const std::string fullscreenFlag = "--screensaver";
        for (int x = 1; x < argc; x++) {
            screensaverMode |= fullscreenFlag == argv[x];
        }
    }

    if(screensaverMode) {
        MainLoop loop(std::make_unique<AsteroidsWorld>(true), true);
        loop.mainLoop();
    } else {
        MainLoop loop(std::make_unique<Menu>());
        loop.mainLoop();
    }

    return 0;
}
