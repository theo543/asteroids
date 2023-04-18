#include "main/MainLoop.h"
#include "main/TestScene.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main() {
    #ifdef __linux__
    XInitThreads();
    #endif

    MainLoop loop(std::make_unique<TestScene>());
    loop.mainLoop();

    return 0;
}
