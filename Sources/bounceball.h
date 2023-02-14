#ifndef __BOUNCEBALL_H__
#define __BOUNCEBALL_H__

#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <time.h>
#include <dwmapi.h>

#include "ball.h"
#include "block.h"
#include "plank.h"
#include "resource.h"

enum Status { START, PLAYING, PAUSE, OVER, WIN, EXIT };

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

class MainGame
{
    private:
        static constexpr auto TITLE = "Bounce Ball";

    private:
        static const int SCREEN_WIDTH = 644;
        static const int SCREEN_HEIGHT = 480;
        static const int GAME_FPS = 60;

    private:
        static const int IS_DARK_MODE = 1;
        static const int DARK_MODE_CODE = 20;

    private:
        static const int TITLE_FONT_SIZE = 35;
        static const int INFO_FONT_SIZE = 15;
        static const int TEXT_BORDER = 5;
        static const int TEXT_LENGTH = 30;
    
    private:
        static const int TITLE_TOP = 150;
        static const int INFO_TOP = 340;
        static const int SCORE_TOP = 250;
        static const int BEST_SCORE_TOP = 270;

    private:
        static const int MAIN_TITLE_WIDTH = 210;
        static const int OVER_TITLE_WIDTH = 175;
        static const int INFO_WIDTH = 195;
        static const int SCORE_WIDTH = 120;
        static const int DURING_SCORE_WIDTH = 90;

    private:
        static const int GAMEOVER_DELAY_DISTANCE = 200;
        static const int CATCH_SCORE = 1;
        static const int HIT_SCORE = 5;

    private:
        static const int WINDOW_BORDER_X = 2;
        static const int WINDOW_BORDER_Y = 3;

    private:
        static constexpr SDL_Color WHITE = { 255, 255, 255 };

    private:
        SDL_Window* window;
        SDL_SysWMinfo sysInfo;
        SDL_Rect screen;
        SDL_Event event;

    private:
        Image image;
        Font font;

    private:
        Ball ball;
        Plank plank;
        Block block[Block::ROWS][Block::COLS];

    private:
        Status status;
        int score;
        int bestScore;
        int hitCount;

    private:
        SDL_RWops* getResource(LPCSTR, LPCSTR);
        SDL_Surface* loadSurface(Uint32);

    private:
        void initBlock();
        void freeImage();
        void freeFont();
        void levelUp();
        void restart();
        void gameover();

    private:
        void displayText(const char*, int, int, TTF_Font*);
        void displayImage();
        void displayInfo();
        void displayPlank();
        void displayBall();
        void displayBlock();

    private:
        void reflect();
        void reflectOnPlank();
        void reflectOnEdge();
        void reflectOnBlock();

    public:
        static Uint32 getDelayTick(Uint32, Uint32);

    public:
        void initWindow();
        void initGame();
        void loadImage();
        void loadFont();

    public:
        void setDarkMode();
        void restoreWindow();

    public:
        bool isRunning();
        void close();
        void update();
        void events();
        void display();
};
#endif