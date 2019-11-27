#include "Box2D/Box2D.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scrolltext.h"
#include <iostream>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTimer>

#define DEGTORAD 0.0174532925199432957f
#define WIDTH 25
#define HEIGHT 40

MainWindow::MainWindow(QWidget *parent, EconEngine* model)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(b2Vec2 (0.0f, 9.81f))

{
    ui->setupUi(this);

    // These are UI connections.
    QObject::connect(ui->startButton, &QPushButton::pressed, this, &MainWindow::on_startButton_clicked);

    QObject::connect(ui->actionMicroeconomics_Rule, &QAction::triggered, this, &MainWindow::redirectKhanAcademy);
    QObject::connect(ui->welcomeCheck4, &QPushButton::clicked, this, &MainWindow::on_welcomeCheck4_clicked);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);
    QObject::connect(this, &MainWindow::sigStartSimulation, model, &EconEngine::onNewDayLemonade);
    QObject::connect(model, &EconEngine::sigSimulationComplete, this, &MainWindow::onSimulationComplete);


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
///Performs a simulation step for box2d world.
/// Updating the position & velocity of all bodies in the world.
/// \brief MainWindow::updateWorld
///
void MainWindow::updateWorld(){
    world.Step(1.0f/60.f, 4, 1);
    //Get position of bodies to update QLabel image positions
    b2Vec2 position = lemonBody->GetPosition();
    b2Vec2 pitchPos = pitcherBody->GetPosition();
     //move images with bodies to visually test
    //height & width is fixed so updating the two will not change the Qlabel
    lemonImage->setGeometry(position.x, position.y, 0, 0);
    pitcherImage->setGeometry(pitchPos.x,pitchPos.y,0,0);

    //check for collision of bodies
    collisionCheck();
    QTimer::singleShot(15,this,&MainWindow::updateWorld);
}
///Creates priv member variable lemonBody in box2d.
/// lemonBody defines a lemon being dropped in our world.
/// \brief MainWindow::createLemonBody
///
void MainWindow::createLemonBody(){
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(375.0f, 0.0f);
    lemonBody = world.CreateBody(&bodyDef);

    //Qlabel created purely for visually testing lemonBody.
    lemonImage = new QLabel();
    lemonImage->setFixedSize(50, 50);
    QPixmap lemonPix("/home/ryan/lemon.png");
    int w = lemonImage->width();
    int h = lemonImage->height();
    layout->addWidget(lemonImage);
    lemWin = new QWidget();
    lemWin->setLayout(layout);
    lemonImage->setPixmap(lemonPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    // Define box shape for dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(WIDTH/2, HEIGHT/2);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.1f;

    // Add the shape to the body.
    lemonBody->CreateFixture(&fixtureDef);

    // Preloads all .png files
    loadStartImages();
}
///Creates priv member variable groundBody in box2d.
/// groundBody defines the ground level for our world.
/// \brief MainWindow::createGroundBody
///
void MainWindow::createGroundBody(){

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(40.0f, 300.0f);
    groundBodyDef.type = b2_staticBody;
    groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;
    groundBox.SetAsBox(375.0f, 2.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 1.0f);

}
///Creates priv member variable pitcherBody in box2d.
/// pitcherBody defines a static body placed on groundBody.
/// \brief MainWindow::createPitcherBody
///
void MainWindow::createPitcherBody(){
    b2BodyDef statTestBodyDef;
    statTestBodyDef.type = b2_staticBody;
    statTestBodyDef.position.Set(375.0f, 300.0f);
    pitcherBody = world.CreateBody(&statTestBodyDef);

    //Qlabel created purely for visually testing pitcherBody.
    pitcherImage = new QLabel();
    pitcherImage->setFixedSize(100, 100);
    QPixmap groundPix("/home/ryan/Pitcher.png");
    int gw = pitcherImage->width();
    int gh = pitcherImage->height();
    layout->addWidget(pitcherImage);
    pitcherImage->setPixmap(groundPix.scaled(gw, gh, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Define the pitcher box shape.
    b2PolygonShape testBox;
    testBox.SetAsBox(WIDTH/2, HEIGHT/2);

    pitcherBody->CreateFixture(&testBox,1.0f);
}

///Checks for the collision of a body with another body
/// using the edges of bodies to detect collision.
/// \brief MainWindow::collisionCheck
///
void MainWindow::collisionCheck(){
    //Gets edge for body and iterates through each edge
    for (b2ContactEdge* edge = lemonBody->GetContactList(); edge; edge = edge->next){
        //check if body is in contact with another body
        if(edge->contact->IsTouching()){
            lemonImage->setPixmap(QPixmap());
             world.DestroyBody(lemonBody);
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    ui->welcomeFrame->setVisible(false);
    ui->dayFrame->setVisible(false);
    ui->progressFrame->setVisible(true);
    ui->progressFrame->raise();

    this->createLemonade();

    changeNewsText("Welcome to Lemonomics! Beware of whales!");
  
    emit sigStartSimulation(this->lemonade);
}

/// Slot used to build a lemonade object based on the values within the UI,
/// and then pass by reference to the data member lemonade.
/// \brief MainWindow::createLemonade
///
void MainWindow::createLemonade(){
    lemWin->show();
    lemWin->setMinimumWidth(750);
    lemWin->setMinimumHeight(750);

    lemonade.setRecipe(ui->LemonSpinBox->value(),
                       ui->sugarSpinBox->value(),
                       ui->iceSpinBox->value(),
                       ui->priceSpinBox->value());

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

///
/// \brief MainWindow::redirectKhanAcademy A method that pops open a hyperlink to khanacademy to learn more about microeconomics.
///
void MainWindow::redirectKhanAcademy()
{
    QMessageBox msgBox;
    msgBox.setText("<a href='https://www.khanacademy.org/economics-finance-domain/microeconomics'>Khan Academy</a> <a href='https://eccles.utah.edu/programs/online-courses/'>UofU Business Courses</a>");
    msgBox.exec();
}

void MainWindow::onSimulationComplete()
{
    this->updateData();
    this->animationForDay();
}

void MainWindow::animationForDay()
{
    QRect dimensions(0, 0, ui->crowdLabel->width(), ui->crowdLabel->height());
    QPixmap defaultImage;
    // TODO: Update the values for checking crowd tiers
    // We have to create a temp pixmap and set it to our default image
    // because there is no obvious way to set a pixmap to a image
    if(game.yesterday().demanded < 50)
    {
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Light.png");
        defaultImage = temp;
    }
    else if(game.yesterday().demanded < 101)
    {
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Medium.png");
        defaultImage = temp;
    }
    else
    {
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Heavy.png");
        defaultImage = temp;
    }
    ui->crowdLabel->setPixmap(defaultImage.copy(dimensions));
}

void MainWindow::loadStartImages()
{
    // QLabel rectangle dimensions, and start x/y coordinate for 1920x1080p images.
    QRect dimensions(350, 100, ui->welcomeBackground->width(), ui->welcomeBackground->height());

    // Creates background color and fills with light blue.
    QPixmap startBackground(ui->welcomeBackground->width(), ui->welcomeBackground->height());
            startBackground.fill(QColor(47, 191, 235));
    QPixmap defaultImage(":/img/Images/Background Default.png");
    QPixmap startLogo(":/img/Images/logo.png");

    // Sets each image to corresponding label.
    ui->welcomeBackground->setPixmap(startBackground);
    ui->welcomeLogo->setPixmap(startLogo);
    ui->simulationPicture->setPixmap(defaultImage.copy(dimensions));

}

void MainWindow::on_welcomeCheck4_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck4->setVisible(false);
    }
}

void MainWindow::on_welcomeCheck3_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck3->setVisible(false);
    }
}

void MainWindow::on_welcomeCheck2_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck2->setVisible(false);
    }
}

void MainWindow::on_day_change(QString scrollText)
{
    this->changeNewsText(scrollText);
}

void MainWindow::changeNewsText(QString scrollText)
{
    QHBoxLayout* layout = new QHBoxLayout(ui->newsWidget);
    ScrollText* news = new ScrollText(ui->newsWidget);
    QFont font("manjari", 20);
    news->setFont(font);
    layout->addWidget(news);
    news->setText(scrollText);
}


