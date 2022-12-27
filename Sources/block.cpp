#include "block.h"

void Block::init(int x, int y)
{
    this->x = x;
    this->y = y;
    this->w = SIZE;
    this->h = SIZE;
    this->isAlive = true;
}

void Block::destoryed()
{
    this->isAlive = false;
}

bool Block::getIsAlive()
{
    return isAlive;
}

int Block::getCenterX()
{
    return this->x + SIZE / 2;
}

int Block::getCenterY()
{
    return this->y + SIZE / 2;
}