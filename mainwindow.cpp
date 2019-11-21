#include "Box2D/Box2D.h"
//#include "lemonade.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent, EconEngine* model)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(b2Vec2 (0.0f, -10.0f))

{
    ui->setupUi(this);

    QObject::connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::on_startButton_clicked);
    QObject::connect(this, &MainWindow::sigStartSimulation, model, &EconEngine::onNewDayLemonade);
    QObject::connect(model, &EconEngine::sigSimulationComplete, this, &MainWindow::onSimulationComplete);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);

    // Connects the Create Lemonade button to the main window.
    // Allows us to build a lemonade object from the values within the UI.
    QObject::connect(ui->CreateLemonadeButton,&QPushButton::pressed,this,&MainWindow::createLemonade);


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
    emit sigNewPos(position.y*100);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);
}

void MainWindow::on_startButton_clicked()
{
    ui->welcomeFrame->setVisible(false);

    this->createLemonade();

    emit sigStartSimulation(this->lemonade);
}
/// Slot used to build a lemonade object based on the values within the UI,
/// and then pass by reference to the data member lemonade.
/// \brief MainWindow::createLemonade
///
void MainWindow::createLemonade(){
    lemonade.setRecipe(ui->LemonSpinBox->value(),
                       ui->sugarSpinBox->value(),
                       ui->iceSpinBox->value(),
                       ui->priceSpinBox->value());
    //Debug info produced below for testing
    qDebug() << lemonade.getSugar();
    qDebug() << lemonade.getLemon();
    qDebug() << lemonade.getIce();
    qDebug() << lemonade.getPricePerCup();

}

/// Uses the lemonade data from yesterday if the user wishes not to change their recipe or price.
/// Sets the values of the spinboxes on the UI to the lemonade data.
/// \brief MainWindow::on_yesterdayButton_clicked
///
void MainWindow::on_yesterdayButton_clicked()
{

    // IDEA: use game.days[currentDay - 1].lemonade to get yesterday's recipe! :)
    ui->LemonSpinBox->setValue(lemonade.getLemon());
    ui->sugarSpinBox->setValue(lemonade.getSugar());
    ui->iceSpinBox->setValue(lemonade.getIce());
    ui->priceSpinBox->setValue(lemonade.getPricePerCup());
    // Keep lemonade on same recipe. Move on.
    qDebug() << lemonade.getSugar();
    qDebug() << lemonade.getLemon();
    qDebug() << lemonade.getIce();
    qDebug() << lemonade.getPricePerCup();

}

void MainWindow::updateData()
{
    ui->profitLabel->setText("Profit: $" + QString::number(game.yesterday().profit));
    ui->salesLabel->setText("Sales: $"   + QString::number(game.yesterday().sales));
    ui->costLabel->setText("Cost: $"     + QString::number(game.yesterday().cost));
    ui->demandLabel->setText("Demand: "  + QString::number(game.yesterday().demanded));
}

void MainWindow::onSimulationComplete()
{
    this->updateData();
}
