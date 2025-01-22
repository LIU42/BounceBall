#ifndef __CORES_HOODLE_H__
#define __CORES_HOODLE_H__

#include "cores/points.h"

class Hoodle : public IntPoint
{
    private:
        int directX;
        int directY;
        int speed;

    public:
        void reset(int x, int y);
        void move();
        void reflectX();
        void reflectY();

    public:
        void setSpeed(int speed);
        void setDirectX(int directX);
        void setDirectY(int directY);

    public:
        int getSpeed();
        int getCenterX();
        int getCenterY();
};
#endif
