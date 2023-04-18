#include "main/MainLoop.h"
#include "world/World.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main() {
    #ifdef __linux__
    XInitThreads();
    #endif

    MainLoop loop(std::make_unique<World>());
    loop.mainLoop();

    return 0;
}
