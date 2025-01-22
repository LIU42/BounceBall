#include <QApplication>

#include "views/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow window;

    window.initializeComponent();
    window.show();

    return application.exec();
}
