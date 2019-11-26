#include "Box2D/Box2D.h"
//#include "lemonade.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <iostream>

#define DEGTORAD 0.0174532925199432957f

MainWindow::MainWindow(QWidget *parent, EconEngine* model)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(b2Vec2 (0.0f, 9.81f))

{
    ui->setupUi(this);

    QObject::connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::on_startButton_clicked);
    //QObject::connect(ui-> startButton, &QPushButton::clicked, model, &EconEngine::onNewDayLemonade);
    //QObject::connect(model, &EconEngine::sigSimulationComplete, model, );
    QTimer::singleShot(30,this,&MainWindow::updateWorld);

    // Connects the Create Lemonade button to the main window.
    // Allows us to build a lemonade object from the values within the UI.
    QObject::connect(ui->CreateLemonadeButton,&QPushButton::pressed,this,&MainWindow::createLemonade);
    layout = new QHBoxLayout();
    createGroundBody();
    createLemonBody();
    createPitcherBody();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateWorld(){
    world.Step(1.0f/60.f, 4, 1);
    // Now print the position and angle of the body.
    b2Vec2 position = lemonBody->GetPosition();
    b2Vec2 vec(0.0f,0.8f);
    lemonBody->ApplyLinearImpulse(vec,lemonBody->GetWorldCenter(),true);
    lemonImage->setGeometry(position.x, position.y, 0, 100);
    //collisionCheck();
    QTimer::singleShot(20,this,&MainWindow::updateWorld);
}
void MainWindow::createLemonBody(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearVelocity = b2Vec2(0.0f,1.0f);
    bodyDef.position.Set(375.0f, 0.0f);
    lemonBody = world.CreateBody(&bodyDef);

    lemonImage = new QLabel();
    lemonImage->setFixedSize(50, 50);
    QPixmap lemonPix("/home/ryan/lemon.png");
    int w = lemonImage->width();
    int h = lemonImage->height();

    layout->addWidget(lemonImage);
    lemWin = new QWidget();
    lemWin->setLayout(layout);
    lemonImage->setPixmap(lemonPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.9f;

    // Add the shape to the body.
    lemonBody->CreateFixture(&fixtureDef);

    lemonBody->SetUserData(lemonImage);
}

void MainWindow::createGroundBody(){
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(40.0f, 400.0f);
    groundBodyDef.type = b2_staticBody;

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    groundBody = world.CreateBody(&groundBodyDef);
    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(375.0f, 2.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 1.0f);
}

void MainWindow::createPitcherBody(){
    b2BodyDef statTestBodyDef;
    statTestBodyDef.type = b2_dynamicBody;
    statTestBodyDef.position.Set(375.0f, 0.0f);

    QLabel *groundLabel = new QLabel();
    groundLabel->setFixedSize(150, 150);
    QPixmap groundPix("/home/ryan/Pitcher.png");
    int gw = groundLabel->width();
    int gh = groundLabel->height();
    layout->addWidget(groundLabel);
    groundLabel->setPixmap(groundPix.scaled(gw, gh, Qt::KeepAspectRatio, Qt::SmoothTransformation));


    pitcherBody = world.CreateBody(&statTestBodyDef);

    b2PolygonShape testBox;
    testBox.SetAsBox(gw, gh);

    b2FixtureDef testFixDef;
    testFixDef.shape = &testBox;
    testFixDef.density = 1.0f;
    testFixDef.friction = 0.1f;
    testFixDef.restitution = 0.9f;

    pitcherBody->CreateFixture(&testFixDef);

    pitcherBody->SetUserData(groundLabel);
}

void MainWindow::collisionCheck(){
    //Gets edge for body and iterates through each edge
    for (b2ContactEdge* edge = lemonBody->GetContactList(); edge; edge = edge->next){
        //check if body is in contact with another body
        if(edge->contact->IsTouching()){
             world.DestroyBody(lemonBody);
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    ui->welcomeFrame->setVisible(false);

    emit sigStartSimulation();
}
/// Slot used to build a lemonade object based on the values within the UI,
/// and then pass by reference to the data member lemonade.
/// \brief MainWindow::createLemonade
///
void MainWindow::createLemonade(){

    lemonade = new Lemonade(ui->LemonSpinBox->value(),ui->sugarSpinBox->value(),ui->iceSpinBox->value(),ui->priceSpinBox->value());
    lemWin->show();
    lemWin->setMinimumWidth(750);
    lemWin->setMinimumHeight(750);

//    for(int i = 0; i < lemonade->getSugar(); i++)
//    {

//    }
//    for(int i = 0; i < lemonade->getLemon(); i++)
//    {

//    }
//    for(int i = 0; i < lemonade->getIce(); i++)
//    {

//    }
//    for(int i = 0; i < lemonade->getPricePerCup(); i++)
//    {

//    }

}



void MainWindow::onGameUpdate(GameState state)
{
    ui->profitLabel->setText("Profit: $" + QString::number(state.day->profit));
    ui->salesLabel->setText("Sales: $" + QString::number(state.day->sales));
    ui->costLabel->setText("Cost: $" + QString::number(state.day->cost));
    ui->demandLabel->setText("Demand: " + QString::number(state.day->demanded));
}

/// Uses the lemonade data from yesterday if the user wishes not to change their recipe or price.
/// Sets the values of the spinboxes on the UI to the lemonade data.
/// \brief MainWindow::on_yesterdayButton_clicked
///
void MainWindow::on_yesterdayButton_clicked()
{
    ui->LemonSpinBox->setValue(lemonade->getLemon());
    ui->sugarSpinBox->setValue(lemonade->getSugar());
    ui->iceSpinBox->setValue(lemonade->getIce());
    ui->priceSpinBox->setValue(lemonade->getPricePerCup());
    // Keep lemonade on same recipe. Move on.
    qDebug() << lemonade->getSugar();
    qDebug() << lemonade->getLemon();
    qDebug() << lemonade->getIce();
    qDebug() << lemonade->getPricePerCup();

}


