#include "Game.h"
#include "TestScene.h"

#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main() {
    #ifdef __linux__
    XInitThreads();
    #endif

    Game game(std::make_unique<TestScene>());
    game.mainLoop();

    return 0;
}
