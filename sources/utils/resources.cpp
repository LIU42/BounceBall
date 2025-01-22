#include "utils/resources.h"

GameResources::GameResources()
{
    hoodlePixmap = new QPixmap(":/images/hoodle.png");
    blockPixmap = new QPixmap(":/images/block.png");
    plankPixmap = new QPixmap(":/images/plank.png");
}

GameResources::~GameResources()
{
    delete hoodlePixmap;
    delete blockPixmap;
    delete plankPixmap;
}

QPixmap* GameResources::getHoodlePixmap()
{
    return hoodlePixmap;
}

QPixmap* GameResources::getBlockPixmap()
{
    return blockPixmap;
}

QPixmap* GameResources::getPlankPixmap()
{
    return plankPixmap;
}
