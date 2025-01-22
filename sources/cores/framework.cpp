#include "cores/framework.h"

GameFramework::GameFramework()
{
    this->hoodle = new Hoodle();

    for (int row = 0; row < SceneProperties::BLOCK_ROWS; row++)
    {
        for (int col = 0; col < SceneProperties::BLOCK_COLS; col++)
        {
            int x = row * BlockProperties::SIZE + SceneProperties::MARGIN_X;
            int y = col * BlockProperties::SIZE + SceneProperties::MARGIN_Y;

            this->blocks[row][col] = new Block(x, y);
        }
    }
    this->plank = new Plank();
}

GameFramework::~GameFramework()
{
    delete hoodle;

    for (int row = 0; row < SceneProperties::BLOCK_ROWS; row++)
    {
        for (int col = 0; col < SceneProperties::BLOCK_COLS; col++)
        {
            delete blocks[row][col];
        }
    }
    delete plank;
}

void GameFramework::initialize()
{
    status = GameStatus::PREPARE;
    thisScore = 0;
    bestScore = 0;
}

void GameFramework::pause()
{
    status = GameStatus::PAUSE;
}

void GameFramework::resume()
{
    status = GameStatus::PLAYING;
}

void GameFramework::restart()
{
    status = GameStatus::PLAYING;
    thisScore = 0;

    int ballX = QRandomGenerator::global()->bounded(BallProperties::START_X_MIN, BallProperties::START_X_MAX);
    int ballY = QRandomGenerator::global()->bounded(BallProperties::START_Y_MIN, BallProperties::START_Y_MAX);

    hoodle->reset(ballX, ballY);

    for (int row = 0; row < SceneProperties::BLOCK_ROWS; row++)
    {
        for (int col = 0; col < SceneProperties::BLOCK_COLS; col++)
        {
            blocks[row][col]->reset();
        }
    }
    plank->reset();
}

void GameFramework::addScore()
{
    this->thisScore += 1;

    if (this->thisScore >= GameProperties::LEVEL3_SCORE)
    {
        hoodle->setSpeed(BallProperties::SPEED3);
    }
    else if (this->thisScore >= GameProperties::LEVEL2_SCORE)
    {
        hoodle->setSpeed(BallProperties::SPEED2);
    }
}

void GameFramework::gameover()
{
    if (hoodle->getCenterY() >= SceneProperties::HEIGHT + GameProperties::GAMEOVER_DELAY_DISTANCE)
    {
        if (thisScore > bestScore)
        {
            bestScore = thisScore;
        }
        status = GameStatus::FAILURE;
    }
    else if (thisScore >= SceneProperties::BLOCK_ROWS * SceneProperties::BLOCK_COLS)
    {
        if (thisScore > bestScore)
        {
            bestScore = thisScore;
        }
        status = GameStatus::SUCCESS;
    }
}

void GameFramework::collision()
{
    collisionOnPlank();
    collisionOnEdge();
    collisionOnBlock();
}

void GameFramework::update(int plankX)
{
    hoodle->move();
    plank->move(plankX);
    gameover();
    collision();
}

void GameFramework::collisionOnEdge()
{
    int ballCenterX = hoodle->getCenterX();
    int ballCenterY = hoodle->getCenterY();

    if (ballCenterX <= BallProperties::RADIUS || ballCenterX >= SceneProperties::WIDTH - BallProperties::RADIUS)
    {
        hoodle->reflectX();
    }
    if (ballCenterY <= BallProperties::RADIUS)
    {
        hoodle->reflectY();
    }
}

void GameFramework::collisionOnPlank()
{
    int ballSpeed = hoodle->getSpeed();

    int ballCenterX = hoodle->getCenterX();
    int ballCenterY = hoodle->getCenterY();

    if (ballCenterY < PlankProperties::START_Y - BallProperties::RADIUS)
    {
        return;
    }
    if (ballCenterY > PlankProperties::START_Y - BallProperties::RADIUS + ballSpeed)
    {
        return;
    }
    if (ballCenterX < plank->getLeftBorder() - ballSpeed)
    {
        return;
    }
    if (ballCenterX > plank->getRightBorder() + ballSpeed)
    {
        return;
    }
    hoodle->reflectY();
}

void GameFramework::collisionOnBlock()
{
    bool hasIntercationalBrick = false;

    int interactionalBrickRow = 0;
    int interactionalBrickCol = 0;

    int interactionalAbsDistanceX = 10000;
    int interactionalAbsDistanceY = 10000;

    for (int row = 0; row < SceneProperties::BLOCK_ROWS; row++)
    {
        for (int col = 0; col < SceneProperties::BLOCK_COLS; col++)
        {
            if (blocks[row][col]->isDestroyed())
            {
                continue;
            }
            int distanceX = hoodle->getCenterX() - blocks[row][col]->getCenterX();
            int distanceY = hoodle->getCenterY() - blocks[row][col]->getCenterY();

            int absDistanceX = qAbs(distanceX);
            int absDistanceY = qAbs(distanceY);

            if (absDistanceX > BallProperties::RADIUS + BlockProperties::SIZE / 2)
            {
                break;
            }
            if (absDistanceY > BallProperties::RADIUS + BlockProperties::SIZE / 2)
            {
                continue;
            }
            if (absDistanceX + absDistanceY < interactionalAbsDistanceX + interactionalAbsDistanceY)
            {
                hasIntercationalBrick = true;

                interactionalBrickRow = row;
                interactionalBrickCol = col;

                interactionalAbsDistanceX = absDistanceX;
                interactionalAbsDistanceY = absDistanceY;
            }
        }
    }
    if (hasIntercationalBrick)
    {
        blocks[interactionalBrickRow][interactionalBrickCol]->destroy();
        addScore();

        if (interactionalAbsDistanceX <= interactionalAbsDistanceY)
        {
            hoodle->reflectY();
        }
        else
        {
            hoodle->reflectX();
        }
    }
}

GameStatus GameFramework::getStatus()
{
    return status;
}

int GameFramework::getThisScore()
{
    return thisScore;
}

int GameFramework::getBestScore()
{
    return bestScore;
}

int GameFramework::getHoodleX()
{
    return hoodle->getX();
}

int GameFramework::getHoodleY()
{
    return hoodle->getY();
}

int GameFramework::getPlankX()
{
    return plank->getX();
}

int GameFramework::getPlankY()
{
    return plank->getY();
}

int GameFramework::getBlockX(int row, int col)
{
    return blocks[row][col]->getX();
}

int GameFramework::getBlockY(int row, int col)
{
    return blocks[row][col]->getY();
}

bool GameFramework::isBlockDestroyed(int row, int col)
{
    return blocks[row][col]->isDestroyed();
}
