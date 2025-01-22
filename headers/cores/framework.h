#ifndef __CORES_FRAMEWORK_H__
#define __CORES_FRAMEWORK_H__

#include <QRandomGenerator>

#include "cores/hoodle.h"
#include "cores/block.h"
#include "cores/plank.h"

#include "utils/constants.h"

enum class GameStatus
{
    PREPARE,
    PLAYING,
    PAUSE,
    FAILURE,
    SUCCESS,
};

class GameFramework
{
    private:
        Block* blocks[SceneProperties::BLOCK_ROWS][SceneProperties::BLOCK_COLS];
        Plank* plank;
        Hoodle* hoodle;

    private:
        GameStatus status;

    private:
        int thisScore;
        int bestScore;

    private:
        void collisionOnEdge();
        void collisionOnBlock();
        void collisionOnPlank();

    private:
        void addScore();
        void gameover();
        void collision();

    public:
        GameFramework();
        ~GameFramework();

    public:
        void initialize();
        void pause();
        void resume();
        void update(int plankX);
        void restart();

    public:
        GameStatus getStatus();

    public:
        int getThisScore();
        int getBestScore();

    public:
        bool isBlockDestroyed(int row, int col);

    public:
        int getHoodleX();
        int getHoodleY();
        int getPlankX();
        int getPlankY();
        int getBlockX(int row, int col);
        int getBlockY(int row, int col);
};
#endif
