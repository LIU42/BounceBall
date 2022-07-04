#ifndef __BOUNCEBALL_H__
#define __BOUNCEBALL_H__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Windows.h>
#include <time.h>

#include "config.h"
#include "resource.h"

using namespace std;

struct Image
{
    SDL_PixelFormat* format;
    SDL_Surface* surface;
    SDL_Surface* plank;
    SDL_Surface* ball;
    SDL_Surface* block;
};

struct Font
{
    TTF_Font* title;
    TTF_Font* info;
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

class MainGame
{
    public:
        HINSTANCE hInstance;
        SDL_Window* window;
        SDL_Rect screen;
        SDL_Event event;

    public:
        Image image;
        Font font;

    public:
        Ball ball;
        Plank plank;
        Block block[BLOCK_ROWS][BLOCK_COLS];

    public:
        int status;
        int score;
        int bestScore;
        int hitCount;

    public:
        SDL_RWops* getResource(HINSTANCE, LPCSTR, LPCSTR);
        SDL_Surface* loadSurface(DWORD);

    public:
        void initWindow();
        void initGame();
        void initBlock();
        void loadImage();
        void loadFont();
        void freeImage();
        void freeFont();
        void close();

    public:
        void levelUp();
        void gameover();
        void update();
        void events();
        void displayText(char*, TTF_Font*, int, int);
        void displayImage();
        void displayInfo();
        void display();
};

extern MainGame game;
#endif