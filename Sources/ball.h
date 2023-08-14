#ifndef __BALL_H__
#define __BALL_H__

#include <SDL.h>
#include <stdlib.h>

class Ball : public SDL_Rect
{
    public:
        static const int RADIUS = 6;
        static const int INIT_LEFT = 100;
        static const int INIT_RIGHT = 544;
        static const int INIT_UPPER = 408;
        static const int INIT_SPEED = 3;
        static const int SPEED_STEP = 500;

    private:
        int signX;
        int signY;
        int speed;

    public:
        void init();
        void move();
        void setLevelUpSpeed(int);
        void setSignX(int);
        void setSignY(int);
        void reflectX();
        void reflectY();

    public:
        int getSpeed();
        int getCenterX();
        int getCenterY();
};
#endif