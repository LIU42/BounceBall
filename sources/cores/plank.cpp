#include "cores/plank.h"

#include "utils/constants.h"

int Plank::bound(int x, int minValue, int maxValue)
{
    if (x < minValue)
    {
        return minValue;
    }
    if (x > maxValue)
    {
        return maxValue;
    }
    return x;
}

void Plank::reset()
{
    this->x = PlankProperties::START_X;
    this->y = PlankProperties::START_Y;
}

void Plank::move(int x)
{
    this->x = bound(x - PlankProperties::WIDTH / 2, 0, SceneProperties::WIDTH - PlankProperties::WIDTH);
}

int Plank::getLeftBorder()
{
    return x;
}

int Plank::getRightBorder()
{
    return x + PlankProperties::WIDTH;
}
