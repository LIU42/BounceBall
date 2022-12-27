#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <SDL.h>

class Block : public SDL_Rect
{
    public:
        static const int ROWS = 32;
        static const int COLS = 10;
        static const int SIZE = 20;

    private:
        bool isAlive;

    public:
        void init(int, int);
        void destoryed();

    public:
        bool getIsAlive();
        int getCenterX();
        int getCenterY();
};
#endif