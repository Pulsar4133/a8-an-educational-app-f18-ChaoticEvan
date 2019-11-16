#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Box2D/Box2D.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      world(b2Vec2 (0.0f, -10.0f))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(this,&MainWindow::newPos,ui->verticalSlider,&QSlider::setValue);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    // Define the ground box shape.
            b2PolygonShape groundBox;

            // The extents are the half-widths of the box.
            groundBox.SetAsBox(50.0f, 10.0f);

            // Add the ground fixture to the ground body.
            groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    body = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9f;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWorld(){
    world.Step(1.0f/60.f, 6, 2);

    // Now print the position and angle of the body.
    b2Vec2 position = body->GetPosition();
    emit newPos(position.y*100);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);
}
