#ifndef __CORES_BRICK_H__
#define __CORES_BRICK_H__

#include "cores/points.h"

class Block : public IntPoint
{
    private:
        bool destroyed;

    public:
        Block(int x, int y);

    public:
        void reset();
        void destroy();

    public:
        bool isDestroyed();

    public:
        int getCenterX();
        int getCenterY();
};
#endif
