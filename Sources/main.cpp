#include "bounceball.h"

int main(int argc, char* argv[])
{
    MainGame game;

    Uint32 startTick = 0;
    Uint32 endTick = 0;

    while (game.isRunning())
    {
        startTick = SDL_GetTicks();
        game.update();
        game.events();
        game.display(); 
        endTick = SDL_GetTicks();    
        game.delay(startTick, endTick);
    }
    return EXIT_SUCCESS;
}