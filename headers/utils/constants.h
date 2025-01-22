#ifndef __UTILS_CONSTANTS_H__
#define __UTILS_CONSTANTS_H__

class BallProperties
{
    public:
        static constexpr int START_X_MIN = 100;
        static constexpr int START_X_MAX = 544;
        static constexpr int START_Y_MIN = 350;
        static constexpr int START_Y_MAX = 408;

    public:
        static constexpr int RADIUS = 6;
        static constexpr int SPEED1 = 3;
        static constexpr int SPEED2 = 4;
        static constexpr int SPEED3 = 5;
};

class BlockProperties
{
    public:
        static constexpr int SIZE = 20;
};

class PlankProperties
{
    public:
        static constexpr int WIDTH = 80;
        static constexpr int HEIGHT = 8;

    public:
        static constexpr int START_X = 282;
        static constexpr int START_Y = 420;
};

class TimerInterval
{
    public:
        static constexpr int FRAME = 16;
};

class GameProperties
{
    public:
        static constexpr int LEVEL2_SCORE = 100;
        static constexpr int LEVEL3_SCORE = 200;

    public:
        static constexpr int GAMEOVER_DELAY_DISTANCE = 200;
};

class SceneProperties
{
    public:
        static constexpr int WIDTH = 644;
        static constexpr int HEIGHT = 480;

    public:
        static constexpr int MARGIN_X = 22;
        static constexpr int MARGIN_Y = 24;

    public:
        static constexpr int BLOCK_ROWS = 30;
        static constexpr int BLOCK_COLS = 10;
};
#endif
