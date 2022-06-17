#include "bounceball.h"

using namespace std;

Window window;
Game game;
Ball ball;
Plank plank;
Block block[BLOCK_ROWS][BLOCK_COLS];

int main(int argc, char* argv[])
{
    Uint32 startTick;
    Uint32 endTick;
    Uint32 delta;

    window.init();
    window.loadImage();
    window.loadFont();
    
    game.init();
    game.initBlock();

    while (game.status != EXIT)
    {
        startTick = SDL_GetTicks();

        game.update();
        game.event();
        game.display();

        endTick = SDL_GetTicks();
        delta = (1000 / GAME_FPS) - (endTick - startTick);
        if (delta >= 0 && delta <= 1000 / GAME_FPS) { SDL_Delay(delta); }
    }
    window.close();
    return 0;
}