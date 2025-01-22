#include "views/widgets/scenewidget.h"

#include "ui_scenewidget.h"

SceneWidget::SceneWidget(QWidget* parent): QWidget(parent), ui(new Ui::SceneWidget)
{
    ui->setupUi(this);

    gameFramework = nullptr;
    gameResources = nullptr;
}

SceneWidget::~SceneWidget()
{
    delete ui;
}

void SceneWidget::setGameFramework(GameFramework* gameFramework)
{
    this->gameFramework = gameFramework;
}

void SceneWidget::setGameResources(GameResources* gameResources)
{
    this->gameResources = gameResources;
}

void SceneWidget::paintHoodle(QPainter& painter)
{
    int x = gameFramework->getHoodleX();
    int y = gameFramework->getHoodleY();

    painter.drawPixmap(x, y, *gameResources->getHoodlePixmap());
}

void SceneWidget::paintPlank(QPainter& painter)
{
    int x = gameFramework->getPlankX();
    int y = gameFramework->getPlankY();

    painter.drawPixmap(x, y, *gameResources->getPlankPixmap());
}

void SceneWidget::paintBlock(QPainter& painter)
{
    for (int row = 0; row < SceneProperties::BLOCK_ROWS; row++)
    {
        for (int col = 0; col < SceneProperties::BLOCK_COLS; col++)
        {
            if (gameFramework->isBlockDestroyed(row, col))
            {
                continue;
            }
            int x = gameFramework->getBlockX(row, col);
            int y = gameFramework->getBlockY(row, col);

            painter.drawPixmap(x, y, *gameResources->getBlockPixmap());
        }
    }
}

void SceneWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    paintHoodle(painter);
    paintPlank(painter);
    paintBlock(painter);
}

void SceneWidget::updateScoreInfo()
{
    ui->scoreValueScore->setNum(gameFramework->getThisScore());
}
