#ifndef __UTILS_RESOURCES_H__
#define __UTILS_RESOURCES_H__

#include <QPixmap>

class GameResources
{
    private:
        QPixmap* hoodlePixmap;
        QPixmap* blockPixmap;
        QPixmap* plankPixmap;

    public:
        GameResources();
        ~GameResources();

    public:
        QPixmap* getHoodlePixmap();
        QPixmap* getBlockPixmap();
        QPixmap* getPlankPixmap();
};
#endif
