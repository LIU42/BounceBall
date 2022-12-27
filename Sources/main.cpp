#include "bounceball.h"

int main(int argc, char* argv[])
{
    MainGame game;

    int startTick;
    int endTick;
    int delayTick;

    srand((unsigned)time(NULL));

    game.initWindow();
    game.initGame();
    game.loadImage();
    game.loadFont();

    while (game.isRunning())
    {
        startTick = SDL_GetTicks();

        game.update();
        game.events();
        game.display();
        
        endTick = SDL_GetTicks();
        delayTick = (1000 / game.FPS) - (endTick - startTick);
        
        SDL_Delay(SDL_max(delayTick, 0));
    }
    game.close();
    return 0;
}