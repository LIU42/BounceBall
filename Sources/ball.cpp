#include "ball.h"

void Ball::init(int screenWidth)
{
    this->x = rand() % (screenWidth - 2 * INIT_MARGIN) + INIT_MARGIN;
    this->y = INIT_TOP;
    this->w = RADIUS * 2;
    this->h = RADIUS * 2;

    this->speed = INIT_SPEED;
    this->signY = -1;

    switch (rand() % 2)
    {
        case 1: signX = 1; break;
        case 0: signX = -1; break;
    }
}

void Ball::move()
{
    this->x += signX * speed;
    this->y += signY * speed;
}

void Ball::speedUp(int score)
{
    speed = INIT_SPEED + score / SPEED_STEP;
}

void Ball::reflectX()
{
    this->signX = -signX;
}

void Ball::reflectY()
{
    this->signY = -signY;
}

void Ball::reflectX(int sign)
{
    this->signX = sign;
}

void Ball::reflectY(int sign)
{
    this->signY = sign;
}

int Ball::getSpeed()
{
    return speed;
}

int Ball::getCenterX()
{
    return this->x + RADIUS;
}

int Ball::getCenterY()
{
    return this->y + RADIUS;
}
