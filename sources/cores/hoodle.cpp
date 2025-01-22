#include "cores/hoodle.h"

#include "utils/constants.h"

void Hoodle::reset(int x, int y)
{
    this->speed = BallProperties::SPEED1;
    this->x = x;
    this->y = y;
    this->directX = +1;
    this->directY = -1;
}

void Hoodle::move()
{
    x += directX * speed;
    y += directY * speed;
}

void Hoodle::reflectX()
{
    directX = -directX;
}

void Hoodle::reflectY()
{
    directY = -directY;
}

void Hoodle::setSpeed(int speed)
{
    this->speed = speed;
}

void Hoodle::setDirectX(int directX)
{
    this->directX = directX;
}

void Hoodle::setDirectY(int directY)
{
    this->directY = directY;
}

int Hoodle::getSpeed()
{
    return speed;
}

int Hoodle::getCenterX()
{
    return x + BallProperties::RADIUS;
}

int Hoodle::getCenterY()
{
    return y + BallProperties::RADIUS;
}
