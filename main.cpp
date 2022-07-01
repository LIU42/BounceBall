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
    INT32 delayTick;

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
        delayTick = (1000 / GAME_FPS) - (endTick - startTick);
        
        SDL_Delay((delayTick > 0) ? delayTick : 0);
    }
    window.close();
    return 0;
}