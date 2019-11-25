#ifndef BOXWORLD_H
#define BOXWORLD_H
#include <QWidget>
#include "lemonade.h"
#include "Box2D/Box2D.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class boxWorld
{
public:
    boxWorld();

private:
    Lemonade *lemonade;
    b2Body *body;
    b2World world;
    QHBoxLayout *layout;
    QLabel *lemonImage;
    QWidget *qw;


};

#endif // BOXWORLD_H
