#include "bounceball.h"

int main(int argc, char* argv[])
{
    MainGame game;

    Uint32 startTick = 0;
    Uint32 endTick = 0;

    srand((unsigned)time(NULL));

    game.initEnvironment();
    game.initWindow();
    game.setDarkMode();
    game.initGame();
    game.loadImage();
    game.loadFont();
    game.restoreWindow();

    while (game.isRunning())
    {
        startTick = SDL_GetTicks();
        game.update();
        game.events();
        game.display(); 
        endTick = SDL_GetTicks();    
        game.delay(startTick, endTick);
    }
    game.close();
    
    return EXIT_SUCCESS;
}