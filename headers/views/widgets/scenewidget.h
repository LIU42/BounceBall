#ifndef __VIEWS_WIDGETS_SCENEWIDGET_H__
#define __VIEWS_WIDGETS_SCENEWIDGET_H__

#include <QWidget>
#include <QPainter>

#include "cores/framework.h"
#include "utils/resources.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class SceneWidget;
}

QT_END_NAMESPACE

class SceneWidget : public QWidget
{
    Q_OBJECT

    private:
        Ui::SceneWidget* ui;

    private:
        GameFramework* gameFramework;
        GameResources* gameResources;

    private:
        void paintHoodle(QPainter& painter);
        void paintPlank(QPainter& painter);
        void paintBlock(QPainter& painter);

    private:
        void paintEvent(QPaintEvent*) override;

    public:
        SceneWidget(QWidget* parent = nullptr);
        ~SceneWidget();

    public:
        void setGameFramework(GameFramework* gameFramework);
        void setGameResources(GameResources* gamrResources);

    public:
        void updateScoreInfo();
};
#endif
