#include "main/MainLoop.h"
#include "main/Menu.h"
#include "resources/GlobalLoaders.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main() {
    #ifdef __linux__
    XInitThreads();
    #endif

    // putting this in main fixes segfault on exit destructor on Linux
    // segfault happens in GlobalLoaders destructor -> sf::Font -> sf::Texture -> sf::GlContext::acquireTransientContext():411 -> access to thread local
    // maybe on Linux static destruction happens in a different thread?
    GlobalLoaders singletonInstance;

    MainLoop loop(std::make_unique<Menu>());
    loop.mainLoop();

    return 0;
}
