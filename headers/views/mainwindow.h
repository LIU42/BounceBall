#ifndef __VIEWS_MAINWINDOW_H__
#define __VIEWS_MAINWINDOW_H__

#include <QMainWindow>
#include <QTimer>
#include <QCursor>

#include "cores/framework.h"
#include "utils/resources.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        Ui::MainWindow* ui;

    private:
        GameFramework* gameFramework;
        GameResources* gameResources;

    private:
        QTimer* frameTimer;

    private:
        void mousePressEvent(QMouseEvent*) override;

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    public:
        void initialize();
};
#endif
