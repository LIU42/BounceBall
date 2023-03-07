#include "plank.h"

void Plank::init(int screenWidth)
{
    SDL_Rect::x = (screenWidth - WIDTH) / 2;
    SDL_Rect::y = INIT_UPPER;
    SDL_Rect::w = WIDTH;
    SDL_Rect::h = HEIGHT;
}

void Plank::moveTo(int mouseX)
{
    SDL_Rect::x = mouseX - WIDTH / 2;
}

int Plank::getLeftBorder()
{
    return SDL_Rect::x;
}

int Plank::getRightBorder()
{
    return SDL_Rect::x + WIDTH;
}