#include "Box2D/Box2D.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent, EconEngine* model)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(b2Vec2 (0.0f, -10.0f))

{
    ui->setupUi(this);

    QObject::connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::on_startButton_clicked);
    //QObject::connect(ui-> startButton, &QPushButton::clicked, model, &EconEngine::onNewDayLemonade);
    //QObject::connect(model, &EconEngine::sigSimulationComplete, model, );
    QObject::connect(ui->actionMicroeconomics_Rule, &QAction::triggered, this, &MainWindow::redirectKhanAcademy);
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

    loadStartImages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWorld(){
    world.Step(1.0f/60.f, 6, 2);

    // Now print the position and angle of the body.
    b2Vec2 position = body->GetPosition();
    emit sigNewPos(position.y*100);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);
}

void MainWindow::on_startButton_clicked()
{
    ui->welcomeFrame->setVisible(false);
    ui->dayFrame->setVisible(true);
    ui->progressFrame->setVisible(false);

    emit sigStartSimulation();
}

void MainWindow::redirectKhanAcademy()
{
    QMessageBox msgBox;
    msgBox.setText("<a href='https://www.khanacademy.org/economics-finance-domain/microeconomics'>Khan Academy</a>");
    msgBox.exec();
}

void MainWindow::onGameUpdate(GameState state)
{
    ui->profitLabel->setText("Profit: $" + QString::number(state.day->profit));
    ui->salesLabel->setText("Sales: $" + QString::number(state.day->sales));
    ui->costLabel->setText("Cost: $" + QString::number(state.day->cost));
    ui->demandLabel->setText("Demand: " + QString::number(state.day->demanded));
}

void MainWindow::loadStartImages()
{
    QPixmap welcomeImage(":/img/Images/Background Default.png");
    ui->welcomePicture->setPixmap(welcomeImage);

}
