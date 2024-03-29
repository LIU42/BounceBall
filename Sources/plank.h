#ifndef __PLANK_H__
#define __PLANK_H__

#include <SDL.h>

class Plank : public SDL_Rect
{
    public:
        static const int WIDTH = 80;
        static const int HEIGHT = 8;
        static const int INIT_X = 282;
        static const int INIT_Y = 420;

    public:
        void init();
        void moveTo(int);

    public:
        int getLeftBorder();
        int getRightBorder();
};
#endif