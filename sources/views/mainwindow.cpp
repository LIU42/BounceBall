#include "views/mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    frameTimer = new QTimer(this);

    gameFramework = new GameFramework();
    gameResources = new GameResources();

    ui->setupUi(this);
    ui->sceneWidget->setGameFramework(gameFramework);
    ui->sceneWidget->setGameResources(gameResources);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameFramework;
    delete gameResources;
    delete frameTimer;
}

void MainWindow::initializeComponent()
{
    gameFramework->initialize();

    connect(frameTimer, &QTimer::timeout, this, [=]
    {
        GameStatus gameStatus = gameFramework->getStatus();

        if (gameStatus == GameStatus::PLAYING)
        {
            gameFramework->update(mapFromGlobal(QCursor::pos()).x());

            if (isMinimized())
            {
                gameFramework->pause();
            }
            ui->sceneWidget->updateScoreInfo();
        }
        else if (gameStatus == GameStatus::FAILURE)
        {
            ui->failureWidget->setThisScore(gameFramework->getThisScore());
            ui->failureWidget->setBestScore(gameFramework->getBestScore());

            ui->mainStackedWidget->setCurrentWidget(ui->failureWidget);
        }
        else if (gameStatus == GameStatus::SUCCESS)
        {
            ui->mainStackedWidget->setCurrentWidget(ui->successWidget);
        }
        ui->sceneWidget->update();
    });

    frameTimer->setInterval(TimerInterval::FRAME);
    frameTimer->start();
}

void MainWindow::mousePressEvent(QMouseEvent*)
{
    GameStatus gameStatus = gameFramework->getStatus();

    if (gameStatus == GameStatus::PAUSE)
    {
        gameFramework->resume();
    }
    else if (gameStatus != GameStatus::PLAYING)
    {
        gameFramework->restart();
        ui->mainStackedWidget->setCurrentWidget(ui->sceneWidget);
    }
}
