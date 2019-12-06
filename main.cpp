#include "Box2D/Box2D.h"
#include "mainwindow.h"
#include <QApplication>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <QApplication>
#include "educationalprompter.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // EPrompt::displayEduPrompt(EPrompt::P_WELCOME);

    return a.exec();
}
