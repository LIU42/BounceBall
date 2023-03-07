#include "block.h"

void Block::init(int x, int y)
{
    SDL_Rect::x = x;
    SDL_Rect::y = y;
    SDL_Rect::w = SIZE;
    SDL_Rect::h = SIZE;
    isAlive = true;
}

void Block::destroyed()
{
    isAlive = false;
}

bool Block::getIsAlive()
{
    return isAlive;
}

int Block::getCenterX()
{
    return SDL_Rect::x + SIZE / 2;
}

int Block::getCenterY()
{
    return SDL_Rect::y + SIZE / 2;
}