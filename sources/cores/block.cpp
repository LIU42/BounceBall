#include "cores/block.h"

#include "utils/constants.h"

Block::Block(int x, int y): IntPoint(x, y)
{
    this->destroyed = false;
}

void Block::reset()
{
    this->destroyed = false;
}

void Block::destroy()
{
    this->destroyed = true;
}

bool Block::isDestroyed()
{
    return destroyed;
}

int Block::getCenterX()
{
    return x + BlockProperties::SIZE / 2;
}

int Block::getCenterY()
{
    return y + BlockProperties::SIZE / 2;
}
