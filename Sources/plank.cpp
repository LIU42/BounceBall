#include "plank.h"

void Plank::init(int screenWidth)
{
    this->x = (screenWidth - WIDTH) / 2;
    this->y = INIT_TOP;
    this->w = WIDTH;
    this->h = HEIGHT;
}

void Plank::moveTo(int mouseX)
{
    this->x = mouseX - WIDTH / 2;
}

int Plank::getLeftBorder()
{
    return this->x;
}

int Plank::getRightBorder()
{
    return this->x + WIDTH;
}