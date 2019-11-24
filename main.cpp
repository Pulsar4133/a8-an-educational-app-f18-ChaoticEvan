#include "Box2D/Box2D.h"
#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


