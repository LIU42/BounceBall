#include "ball.h"

void Ball::init(int screenWidth)
{
    SDL_Rect::x = rand() % (screenWidth - 2 * INIT_MARGIN) + INIT_MARGIN;
    SDL_Rect::y = INIT_UPPER;
    SDL_Rect::w = RADIUS * 2;
    SDL_Rect::h = RADIUS * 2;

    speed = INIT_SPEED;
    signY = -1;

    switch (rand() % 2)
    {
        case 1: signX = 1; break;
        case 0: signX = -1; break;
    }
}

void Ball::move()
{
    SDL_Rect::x += signX * speed;
    SDL_Rect::y += signY * speed;
}

void Ball::speedUp(int score)
{
    speed = INIT_SPEED + score / SPEED_STEP;
}

void Ball::setSignX(int sign)
{
    signX = sign;
}

void Ball::setSignY(int sign)
{
    signY = sign;
}

void Ball::reflectX()
{
    signX = -signX;
}

void Ball::reflectY()
{
    signY = -signY;
}

int Ball::getSpeed()
{
    return speed;
}

int Ball::getCenterX()
{
    return SDL_Rect::x + RADIUS;
}

int Ball::getCenterY()
{
    return SDL_Rect::y + RADIUS;
}
