#ifndef __BALL_H__
#define __BALL_H__

#include <SDL.h>
#include <stdlib.h>

class Ball : public SDL_Rect
{
    public:
        static const int RADIUS = 6;
        static const int INIT_MARGIN = 100;
        static const int INIT_TOP = 408;
        static const int INIT_SPEED = 3;
        static const int SPEED_STEP = 500;

    private:
        int signX;
        int signY;
        int speed;

    public:
        void init(int);
        void move();
        void speedUp(int);
        void reflectX();
        void reflectY();
        void reflectX(int);
        void reflectY(int);

    public:
        int getSpeed();
        int getCenterX();
        int getCenterY();
};
#endif