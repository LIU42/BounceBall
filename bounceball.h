#ifndef __BOUNCEBALL_H__
#define __BOUNCEBALL_H__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include <random>

#include "config.h"
#include "resource.h"

using namespace std;

class Window
{
    public:
        HINSTANCE hInstance;
        SDL_Window* window;
        SDL_Surface* surface;
        SDL_Rect screenRect;
        SDL_Event event;
        TTF_Font* titleFont;
        TTF_Font* infoFont;
        SDL_PixelFormat* format;

    public:
        SDL_Surface* plankImg;
        SDL_Surface* ballImg;
        SDL_Surface* blockImg;

    public:
        SDL_RWops* getResource(HINSTANCE, LPCSTR, LPCSTR);
        SDL_Surface* loadSurface(DWORD);

    public:
        void text(const char*, TTF_Font*, int, int);
        void init();
        void loadImage();
        void loadFont();
        void freeImage();
        void freeFont();
        void close();
};

class Game
{
    public:
        int status;
        int score;
        int bestScore;
        int hitBlockCount;
        char text[TEXT_MAX_LEN];

    public:
        default_random_engine random;
        bernoulli_distribution randSign;
        uniform_int_distribution <int> randX;

    public:
        Game();
        void init();
        void initBlock();
        void levelUp();
        void gameover();
        void update();
        void event();
        void displayImage();
        void displayInfo();
        void display();
};

class Plank
{
    public:
        SDL_Rect rect;
    
    public:
        void init();
        void move(int);
        void display();
};

class Ball
{
    public:
        SDL_Rect rect;
        int signX;
        int signY;
        int speed;

    public:
        void init();
        void move();
        void reflectOnPlank();
        void reflectOnEdge();
        void reflectOnBlock();
        void display();
};

class Block
{
    public:
        SDL_Rect rect;
        bool alive;
    
    public:
        void init(int, int);
        void display();
};

extern Window window;
extern Game game;
extern Ball ball;
extern Plank plank;
extern Block block[BLOCK_ROWS][BLOCK_COLS];
#endif