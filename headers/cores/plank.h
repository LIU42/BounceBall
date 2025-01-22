#ifndef __CORES_PLANK_H__
#define __CORES_PLANK_H__

#include <QtMinMax>

#include "cores/points.h"

class Plank : public IntPoint
{
    private:
        int bound(int x, int minValue, int maxValue);

    public:
        void reset();
        void move(int x);

    public:
        int getLeftBorder();
        int getRightBorder();
};
#endif